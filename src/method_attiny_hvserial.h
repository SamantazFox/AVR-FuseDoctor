#ifndef __PROGRAM_TINY_HVSERIAL_H__
#define __PROGRAM_TINY_HVSERIAL_H__


#include <stdint.h>

#define  HFUSE  0x747C
#define  LFUSE  0x646C
#define  EFUSE  0x666E


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

void readFuses (void)
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

uint16_t readSignature (void)
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


#endif  /* !__PROGRAM_TINY_HVSERIAL_H__ */
