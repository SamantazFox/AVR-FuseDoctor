// AVR High-voltage Serial Fuse Reprogrammer
// Adapted from code and design by Paul Willoughby 03/20/2010
//   http://www.rickety.us/2010/03/arduino-avr-high-voltage-serial-programmer/
//
// Fuse Calc:
//   <a rel="nofollow">http://www.rickety.us/2010/03/arduino-avr-high-vo....</a>


#include <stdint.h>


#define  RST     13    // Output to level shifter for !RESET from transistor
#define  SCI     12    // Target Clock Input
#define  SDO     11    // Target Data Output
#define  SII     10    // Target Instruction Input
#define  SDI      9    // Target Data Input
#define  VCC      8    // Target VCC

#define  HFUSE  0x747C
#define  LFUSE  0x646C
#define  EFUSE  0x666E

// Define ATTiny series signatures
#define  ATTINY13   0x9007  // L: 0x6A, H: 0xFF             8 pin
#define  ATTINY24   0x910B  // L: 0x62, H: 0xDF, E: 0xFF   14 pin
#define  ATTINY25   0x9108  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY44   0x9207  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY45   0x9206  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY84   0x930C  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY85   0x930B  // L: 0x62, H: 0xDF, E: 0xFF    8 pin


uint8_t shiftOut (uint8_t val1, uint8_t val2)
{
	uint16_t inBits = 0;

	//Wait until SDO goes high
	while (!digitalRead(SDO));

	uint16_t dout = (uint16_t) val1 << 2;
	uint16_t iout = (uint16_t) val2 << 2;

	for (uint8_t ii = 10; ii >= 0; ii--)
	{
		digitalWrite(SDI, !!(dout & (1 << ii)));
		digitalWrite(SII, !!(iout & (1 << ii)));
		inBits <<= 1;
		inBits |= digitalRead(SDO);
		digitalWrite(SCI, HIGH);
		digitalWrite(SCI, LOW);
	}

	return inBits >> 2;
}

void writeFuse (uint16_t fuse, uint8_t val)
{
	shiftOut(0x40, 0x4C);
	shiftOut( val, 0x2C);
	shiftOut(0x00, (uint8_t) (fuse >> 8));
	shiftOut(0x00, (uint8_t) fuse);
}

void readFuses ()
{
	uint8_t val;

	shiftOut(0x04, 0x4C);  // LFuse
	shiftOut(0x00, 0x68);

	val = shiftOut(0x00, 0x6C);
	//Serial.print("LFuse: ");
	//Serial.print(val, HEX);

	shiftOut(0x04, 0x4C);  // HFuse
	shiftOut(0x00, 0x7A);

	val = shiftOut(0x00, 0x7E);
	//Serial.print(", HFuse: ");
	//Serial.print(val, HEX);

	shiftOut(0x04, 0x4C);  // EFuse
	shiftOut(0x00, 0x6A);

	val = shiftOut(0x00, 0x6E);
	//Serial.print(", EFuse: ");
	//Serial.println(val, HEX);
}

uint16_t readSignature ()
{
	uint16_t sig = 0;
	uint8_t val;

	for (uint8_t ii = 1; ii < 3; ii++)
	{
		shiftOut(0x08, 0x4C);
		shiftOut(  ii, 0x0C);
		shiftOut(0x00, 0x68);
		val = shiftOut(0x00, 0x6C);
		sig = (sig << 8) + val;
	}

	return sig;
}

void chipErase(void)
{
	//Serial.print ("Erasing chip....");
	shiftOut(0x80, 0x4C);
	shiftOut(0x00, 0x64);
	shiftOut(0x00, 0x6C);
	//Serial.println ("done");
}



void setup()
{
	pinMode(VCC, OUTPUT);
	pinMode(RST, OUTPUT);
	pinMode(SDI, OUTPUT);
	pinMode(SII, OUTPUT);
	pinMode(SCI, OUTPUT);
	pinMode(SDO, OUTPUT);     // Configured as input when in programming mode

	digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V
	//Serial.begin(19200);
}

void loop()
{
	//if (Serial.available() > 0)
	{
		//Serial.read();

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
	}
}