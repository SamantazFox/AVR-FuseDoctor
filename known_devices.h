#ifndef __KNOWN_DEVICES_H__
#define __KNOWN_DEVICES_H__


#include <stdint.h>
#include <avr/pgmspace.h>


typedef enum
{
	TINY_HVSERIAL,
	TINY_PARALLEL,
	MEGA_PARALLEL,
}
progtype_e;


typedef struct
{
	uint16_t    signature;
	uint8_t     lfuse;
	uint8_t     hfuse;
	uint8_t     efuse;
	progtype_e  method;
}
known_device_t;


const PROGMEM known_device_t list_of_mcus[] = {
	//
	// ATtiny requiring HV serial programming
	//
	{ 0x9007, 0x6A, 0xFF, 0x00, TINY_HVSERIAL },  // ATtiny13/13V

	{ 0x910B, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny24/24A
	{ 0x9207, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny44/44A
	{ 0x930C, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny84/84A

	{ 0x9108, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny25
	{ 0x9206, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny45
	{ 0x930B, 0x62, 0xDF, 0xFF, TINY_HVSERIAL },  // ATtiny85

	//
	// ATtiny requiring parallel programming
	//
	{ 0x910A, 0x62, 0xDF, 0xFF, TINY_PARALLEL },  // ATtiny2313A
	{ 0x920D, 0x62, 0xDF, 0xFF, TINY_PARALLEL },  // ATtiny4313A

	//
	// ATmega requiring parallel programming
	//
	{ 0x9403, 0xE1, 0x99, 0x00, MEGA_PARALLEL },  // ATmega16A

	{ 0x9389, 0x5E, 0xD9, 0xF4, MEGA_PARALLEL },  // ATmega8U2
	{ 0x9489, 0x5E, 0xD9, 0xF4, MEGA_PARALLEL },  // ATmega16U2
	{ 0x958A, 0x5E, 0xD9, 0xF4, MEGA_PARALLEL },  // ATmega32U2

	{ 0x9205, 0x62, 0xDF, 0xFF, MEGA_PARALLEL },  // ATmega48A
	{ 0x920A, 0x62, 0xDF, 0xFF, MEGA_PARALLEL },  // ATmega48PA
	{ 0x930A, 0x62, 0xDF, 0xF9, MEGA_PARALLEL },  // ATmega88A
	{ 0x930F, 0x62, 0xDF, 0xF9, MEGA_PARALLEL },  // ATmega88PA
	{ 0x9406, 0x62, 0xDF, 0xF9, MEGA_PARALLEL },  // ATmega168A
	{ 0x940B, 0x62, 0xDF, 0xF9, MEGA_PARALLEL },  // ATmega168PA
	{ 0x9514, 0x62, 0xD9, 0xFF, MEGA_PARALLEL },  // ATmega328
	{ 0x950F, 0x62, 0xD9, 0xFF, MEGA_PARALLEL },  // ATmega328P

	{ 0x9608, 0x62, 0x99, 0xFF, MEGA_PARALLEL },  // ATmega640
	{ 0x9703, 0x62, 0x99, 0xFF, MEGA_PARALLEL },  // ATmega1280
	{ 0x9704, 0x62, 0x99, 0xFF, MEGA_PARALLEL },  // ATmega1281
	{ 0x9801, 0x62, 0x99, 0xFF, MEGA_PARALLEL },  // ATmega2560
	{ 0x9802, 0x62, 0x99, 0xFF, MEGA_PARALLEL },  // ATmega2561
};


#endif  /* !__KNOWN_DEVICES_H__ */
