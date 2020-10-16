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


extern const PROGMEM known_device_t list_of_mcus[];
extern const PROGMEM size_t mcu_list_length;


#endif  /* !__KNOWN_DEVICES_H__ */
