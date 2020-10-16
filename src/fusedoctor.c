// AVR High-voltage Serial Fuse Reprogrammer
// Adapted from code and design by Paul Willoughby 03/20/2010
//   http://www.rickety.us/2010/03/arduino-avr-high-voltage-serial-programmer/
//
// Fuse Calc:
//   <a rel="nofollow">http://www.rickety.us/2010/03/arduino-avr-high-vo....</a>



#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "pindefs.h"
#include "known_devices.h"
#include "methods.h"


int main(void)
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
		// Try to program MCUs supportin "HV serial" method
		attiny_hvserial__detectAndFlash();

		// Wait between each try
		_delay_ms(10000);
	}
}
