# Optimized Code for Grove LED Bar v2.0 with MY9221 Controller

<img src="https://github.com/mumanchu/mumanchu/blob/main/assets/MY9221LedBar/my9221-led-bar.jpg" alt="Picture of Grove LED Bar board" width="300">

Please email me if you have any questions or \[polite\] suggestions, info@muman.ch 

## Blurb
This is a 10-segment led bar display with a fast 2-wire serial interface using data and clock signals. It is unusual in that the data is clocked in on the rising AND the falling edges of the 
clock.

Several modules can be cascaded together by connecting the data and clock outputs of one to the data and clock inputs of the next. In practice it is best to limit it to two modules (stereo) because each module requires 208 bits of (bit-banged) data, and the routines block until it's all sent. 

The sent data consists of a 16-bit command, followed by 12x16-bit led data values. The led bar display only needs 8 bits of data per led, but you must still send 16 bits - only the lower 8 bits are used. 

The 10-segment led module only needs ten values, but the MY9221 chip supports 12 data values, so 12 values are always sent but the last two values are ignored.

The 8-bit data value for each led is the 'brightness', where 0=off, 255=full on. It is not a linear scale, and small changes make no noticeable difference.

This code uses fast digital I/O provided by mumanchu's 'Optimized GPIO' library which you can get here: \
https://github.com/mumanchu/OptimizedGPIO \
This is up to 3 times faster than using the standard `digitalWrite()` methods. Using optimised GPIO on a 72MHz processor it takes 360us 
to update one display. Using the standard `digitalWrite()` it takes 880us.

> [!WARNING]
> The module runs on 3.3V or 5V. If using a 3.3V module you must connect the VCC pin to 3.3V, NOT TO 5V! \
> At full brightness, each module uses 250mA. With two modules that's HALF AN AMP! Most microcontroller boards cannot supply that much power, so keep the display brightness really low (e.g. 8) or use a separate 3.3 or 5V power supply.

## Class Reference

Please read the commented source code and example sketch for more details.

```cpp
class MY9221LedBar
{
public:
	bool begin(byte dataPin, byte clockPin);
	void setAllLeds(byte brightness, byte module = 0);
	void setLevel(uint value, byte brightness, byte module = 0);
	void setLed(byte led, byte brightness, byte module = 0);
	void refreshDisplays();
};
```

## Useful Links

**Product** \
https://www.seeedstudio.com/Grove-LED-Bar-v2-0.html

**Wiki** \
https://wiki.seeedstudio.com/Grove-LED_Bar/

**MY9221 Controller Data Sheet** \
https://files.seeedstudio.com/wiki/Grove-LED_Bar/res/MY9221_DS_1.0.pdf

## Revision History

| Date       | Version  | Details |
|:---------- |:---------|:----------- |
| 2026.06.20 | 0.0.0	| Preliminary |

<br/>

## Joke of the Week

_**Time does not exist!!!**_ \
_I can prove it in 5 minutes..._


