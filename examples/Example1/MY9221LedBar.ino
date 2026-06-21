/////////////////////////////////////////////////////////////////////
// Example sketch for the Grove LED Bar v2.0 with MY9221 Controller
// github.com/mumanchu/MY9221LedBar

// (these may already be defined)
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#include <OptimizedGPIO.h>

#define MY9221_NUMBER_OF_MODULES 1	// could be 2 if cascaded
#include "MY9221LedBar.h"
MY9221LedBar led;


void setup() 
{
	// use different pins for RX/TX
	// this is only for the STM32 Nucleo-64 boards
	//Serial.setTx(PC_10);
	//Serial.setRx(PC_11);

	Serial.begin(115200);
	delay(3000);
	Serial.println("\n\rStarted\n\r");
	Serial.flush();

	pinMode(LED_BUILTIN, OUTPUT);

	if (!led.begin(D3, D2)) {
		Serial.println("led.begin() failed");
		Serial.flush();
		while (1)
			yield();
	}
}

void loop() 
{
	ulong t = millis();

	// every 200 millisecnds
	static ulong t1 = 0;
	if (t - t1 >= 200) {
		t1 = t;

		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

		// keep the brightness low to reduce the power
		static byte brightness = 8;

		led.setLevel(rand() % 11, brightness, 0);

		// uncomment this if using two cascaded modules
		// and set MY9221_NUMBER_OF_MODULES = 2, see above
		//led.setLevel(rand() % 11, brightness, 1);

		led.refreshDisplays();
	}
}
