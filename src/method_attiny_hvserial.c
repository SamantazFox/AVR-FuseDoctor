#include "methods.h"

#include <stdint.h>

#define  CMD_HFUSE  0x747C
#define  CMD_LFUSE  0x646C
#define  CMD_EFUSE  0x666E


/*
 * Static functions specific to this programming method
*/

static uint8_t shiftOut (uint8_t val1, uint8_t val2)
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

static void writeFuse (uint16_t fuse, uint8_t val)
{
	shiftOut(0x40, 0x4C);
	shiftOut( val, 0x2C);
	shiftOut(0x00, (uint8_t) (fuse >> 8));
	shiftOut(0x00, (uint8_t) fuse);
}

static void readFuses (void)
{
	//uint8_t val;

	shiftOut(0x04, 0x4C);  // LFuse
	shiftOut(0x00, 0x68);

	//val = shiftOut(0x00, 0x6C);
	//Serial.print("LFuse: ");
	//Serial.print(val, HEX);
	_delay_us(5);

	shiftOut(0x04, 0x4C);  // HFuse
	shiftOut(0x00, 0x7A);

	//val = shiftOut(0x00, 0x7E);
	//Serial.print(", HFuse: ");
	//Serial.print(val, HEX);
	_delay_us(5);

	shiftOut(0x04, 0x4C);  // EFuse
	shiftOut(0x00, 0x6A);

	//val = shiftOut(0x00, 0x6E);
	//Serial.print(", EFuse: ");
	//Serial.println(val, HEX);
	_delay_us(5);
}

static uint16_t readSignature (void)
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

static void chipErase (void)
{
	//Serial.print ("Erasing chip....");
	shiftOut(0x80, 0x4C);
	shiftOut(0x00, 0x64);
	shiftOut(0x00, 0x6C);
	//Serial.println ("done");
}


/*
 * Main routine, called from main
*/

void attiny_hvserial__detectAndFlash (void)
{
	// Enter programming mode
	pinMode(SDO, OUTPUT);     // Set SDO to output

	digitalWrite(SDI, LOW);   // Set "Prog_enable[0]" to 0 (for 8-pins ATtinys)
	digitalWrite(SII, LOW);   // Set "Prog_enable[1]" to 0 (for 8-pins ATtinys)
	digitalWrite(SDO, LOW);   // Set "Prog_enable[2]" to 0 (for 8-pins ATtinys)

	digitalWrite(RST, HIGH);  // 12v Off
	digitalWrite(VCC, HIGH);  // Vcc On
	_delay_us(20);

	digitalWrite(RST, LOW);   // 12v On
	_delay_us(10);

	pinMode(SDO, INPUT);      // Set SDO back to input
	_delay_us(300);

	// Read signature
	uint16_t sig = readSignature();

	// Browse signatures
	for (int i = 0; i < mcu_list_length; i++)
	{
		// Make sure to avoid MCUs not supporting this mode
		if (list_of_mcus[i].method != TINY_HVSERIAL) continue;

		// Check signature, and flash in case of match
		if (list_of_mcus[i].signature == sig)
		{
			// Write low fuse
			writeFuse(CMD_LFUSE, list_of_mcus[i].lfuse);

			// Write high fuse
			writeFuse(CMD_HFUSE, list_of_mcus[i].hfuse);

			// If MCU has it, write extended fuse
			if (list_of_mcus[i].efuse != 0x00)
				writeFuse(CMD_EFUSE, list_of_mcus[i].efuse);
		}
	}

	// Exit programming mode
	digitalWrite(SCI, LOW);
	digitalWrite(VCC, LOW);    // Vcc Off
	digitalWrite(RST, HIGH);   // 12v Off
}
