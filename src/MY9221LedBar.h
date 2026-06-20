#pragma once

/////////////////////////////////////////////////////////////////////
// Grove LED Bar v2.0 with MY9221 Controller
// Copyright (C) muman.ch + github.com/mumanchu, 2026.06.20
// All Rights Reversed Inversely
/*
BLURB

This is a 10-segment led bar display with a fast 2-wire serial 
interface using data and clock signals. It is unusual in that the 
data is clocked in on the rising AND the falling edges of the 
clock.

Several modules can be cascaded together by connecting the data 
and clock outputs of one to the data and clock inputs of the next. 
In practice it is best to limit it to two modules (stereo) because 
each module requires 208 bits of (bit-banged) data, and the 
routines block until it's all sent. 

The sent data consists of a 16-bit command, followed by 12x16-bit 
led data values. The led bar display only needs 8 bits of data per
led, but you must still send 16 bits - only the lower 8 bits are 
used. 

The 10-segment led module only needs ten values, but the MY9221
chip supports 12 data values, so 12 values are always sent but the
last two values are ignored.

The 8-bit data value for each led is the 'brightness', where 0=off, 
255=full on. It is not a linear scale, and small changes make no 
noticeable difference.

This code uses fast digital I/O provided by mumanchu's 'Optimized 
GPIO' library which you can get here:
https://github.com/mumanchu/OptimizedGPIO
This is up to 3 times faster than using the standard digitalWrite() 
methods. Using optimised GPIO on a 72MHz processor it takes 360us 
to update one display. Using the standard digitalWrite() it takes 
880us.

!!WARNINGS!!

The module runs on 3.3V or 5V. If using a 3.3V module you must 
connect the VCC pin to 3.3V, NOT TO 5V!
At full brightness, each module uses 250mA. With two modules that's
HALF AN AMP! Most microcontroller boards cannot supply that much 
power, so keep the display brightness low (e.g. 8) or use a 
separate 3.3 or 5V power supply.

PRODUCT
https://www.seeedstudio.com/Grove-LED-Bar-v2-0.html

WIKI
https://wiki.seeedstudio.com/Grove-LED_Bar/

MY9221 CONTROLLER DATA SHEET
https://files.seeedstudio.com/wiki/Grove-LED_Bar/res/MY9221_DS_1.0.pdf
*/

#include <OptimizedGPIO.h>

// Modules can be chained together (stereo), 
// but more that two is not practical
#ifndef MY9221_NUMBER_OF_MODULES
#define MY9221_NUMBER_OF_MODULES 1
#endif


class MY9221LedBar
{
	OptimizedGPIO data;
	OptimizedGPIO clock;
	byte ledData[MY9221_NUMBER_OF_MODULES * 12];

public:
	bool begin(byte dataPin, byte clockPin);
	void setAllLeds(byte brightness, byte module = 0);
	void setLevel(uint value, byte brightness, byte module = 0);
	void setLed(byte led, byte brightness, byte module = 0);
	void refreshDisplays();

protected:
	void send16Bits(uint16_t value);
	void latch();
};


// Initialize (there is no constructor)
bool MY9221LedBar::begin(byte dataPin, byte clockPin)
{
	if (!data.begin(dataPin, OUTPUT))
		return false;
	if (!clock.begin(clockPin, OUTPUT))
		return false;
	memset(ledData, 0, sizeof(ledData));
	return true;
}

// Set the brightness of all leds, 0=off, 255=full on
void MY9221LedBar::setAllLeds(byte brightness, byte module/*=0*/)
{
	for (int i = 0; i < 10; ++i)
		setLed(i, brightness, module);
}

// Set a level 0..10 (0=off) with the given brightness 0..255
// 0=all leds off, 10=all leds on
void MY9221LedBar::setLevel(uint value, byte brightness, byte module/*=0*/)
{
	for (uint i = 1; i <= 10; ++i)
		setLed(i - 1, value >= i ? brightness : 0, module);
}

// Set an individual led brightness: 0=off, 255=full on
// led: 9=red, 8=yellow, 7..0=green
void MY9221LedBar::setLed(byte led, byte brightness, byte module/*=0*/)
{
	uint n = (((MY9221_NUMBER_OF_MODULES - 1) - module) * 12) + (9 - led);
	if (led > 9 || n >= sizeof(ledData))
		return;
	ledData[n] = brightness;
}

// Send the values to all the displays
// nothing happens unless this is called
void MY9221LedBar::refreshDisplays()
{
	// send the data for all modules
	byte* p = ledData;
	for (int i = 0; i < MY9221_NUMBER_OF_MODULES; ++i) {
		send16Bits(0);		// command = 0
		for (int j = 0; j < 12; ++j)
			send16Bits(*p++);
	}
	// latch the data to the outputs
	latch();
}

// Clock out 16 bits, MS bit first, on rising and falling edge of clock
// the clock pin must be low on exit
void MY9221LedBar::send16Bits(uint16_t value)
{
	bool clk = true;
	uint16_t mask = 0x8000;
	while (mask) {
		data.write(value & mask);
		clock.write(clk);
		clk = !clk;
		mask >>= 1;
	}
}

// Clock the data received by the chip to its led outputs
void MY9221LedBar::latch()
{
	data.reset();
	delayMicroseconds(230);
	for (int i = 0; i < 4; ++i) {
		data.set();
		data.reset();
		delayMicroseconds(1);
	}
}
