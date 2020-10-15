// AVR High-voltage Serial Fuse Reprogrammer
// Adapted from code and design by Paul Willoughby 03/20/2010
//   http://www.rickety.us/2010/03/arduino-avr-high-voltage-serial-programmer/
//
// Fuse Calc:
//   <a rel="nofollow">http://www.rickety.us/2010/03/arduino-avr-high-vo....</a>


#include <stdint.h>

#include "known_devices.h"
#include "method_attiny_hvserial.h"


void main(void)
{
	// Init
	pinMode(VCC, OUTPUT);
	pinMode(RST, OUTPUT);
	pinMode(SDI, OUTPUT);
	pinMode(SII, OUTPUT);
	pinMode(SCI, OUTPUT);
	pinMode(SDO, OUTPUT);     // Configured as input when in programming mode

	digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V


	// Infinite loop
	while (1)
	{
		pinMode(SDO, OUTPUT);     // Set SDO to output

		digitalWrite(SDI, LOW);
		digitalWrite(SII, LOW);
		digitalWrite(SDO, LOW);
		digitalWrite(RST, HIGH);  // 12v Off
		digitalWrite(VCC, HIGH);  // Vcc On
		delayMicroseconds(20);

		digitalWrite(RST, LOW);   // 12v On
		delayMicroseconds(10);

		pinMode(SDO, INPUT);      // Set SDO to input
		delayMicroseconds(300);

		//Serial.println("Reading: ");
		uint16_t sig = readSignature();

		//Serial.print("Signature is: ");
		//Serial.println(sig, HEX);

		readFuses();
		// chipErase();

		if (sig == ATTINY13)
		{
			writeFuse(LFUSE, 0x6A);
			writeFuse(HFUSE, 0xFF);
		}
		else if (sig == ATTINY24 || sig == ATTINY44 || sig == ATTINY84 ||
		         sig == ATTINY25 || sig == ATTINY45 || sig == ATTINY85)
		{
			writeFuse(LFUSE, 0xE2); // writeFuse(LFUSE, 0x62);
			writeFuse(HFUSE, 0xDF); // Erase EEPROM
			//writeFuse(HFUSE, 0xDE); // Retain EEPROM
			writeFuse(EFUSE, 0xFF);
		}

		readFuses();
		digitalWrite(SCI, LOW);
		digitalWrite(VCC, LOW);    // Vcc Off
		digitalWrite(RST, HIGH);   // 12v Off


		delay(10);
	}
}
