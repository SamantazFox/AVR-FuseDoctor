#ifndef __PINDEFS_H__
#define __PINDEFS_H__


/*
 * Common pinouts
*/

// Output to level shifter for !RESET from transistor
#define  PORT_RST      A
#define  PIN_RST       1

// Target VCC
#define  PORT_VCC      A
#define  PIN_VCC       0



/*
 * Pinouts for HV serial programming
*/

// Target Clock Input
#define  PORT_SCI      B
#define  PIN_SCI       2

// Target Data Output
#define  PORT_SDO      B
#define  PIN_SDO       3


// Target Instruction Input
#define  PORT_SII      B
#define  PIN_SII       0

// Target Data Input
#define  PORT_SDI      B
#define  PIN_SDI       1



/*
 * Pinouts for HV parallel programming (control signals)
*/

// Target Clock Input
#define PORT_XTAL1     D
#define PIN_XTAL1      2

// Target Data Output
#define PORT_RDY       B
#define PIN_RDY        3


// Write pulse (active low)
#define PORT_WR        D
#define PIN_WR         4

// Output enable (active low)
#define PORT_OE        D
#define PIN_OE         5

// Byte Select 1
#define PORT_BS1       D
#define PIN_BS1        3

// Byte Select 2
#define PORT_BS2       D
#define PIN_BS2        6

// XTAL Action Bit 0
#define PORT_XA0       D
#define PIN_XA0        1

// XTAL Action Bit 1
#define PORT_XA1       D
#define PIN_XA1        0



/*
 * Pinouts for HV parallel programming (data signals)
*/

// HC595 serial in (data out, same pin as SDI)
#define PORT_HC_DATA   PORT_SDI
#define PIN_HC_DATA    PIN_SDI

// HC595 serial clock (same pin as SCI)
#define PORT_HC_CLK    PORT_SCI
#define PIN_HC_CLK     PIN_SCI

// HC595 "store data" ("shift load"/"RCLK")
#define PORT_HC_LOAD   B
#define PIN_HC_LOAD    4

// HC595 output enable
#define PORT_HC_OE     B
#define PIN_HC_OE      5



/*
 * Pinouts for status LEDS
*/

// Status good/polling LED (Green LED)
#define PORT_LED_G     B
#define PIN_LED_G      6

// Error LED (Red)
#define PORT_LED_R     B
#define PIN_LED_R      7



/*
 * Some macros to emulate Arduino functions
*/

#define LOW  0
#define HIGH 1

#define INPUT  0
#define OUTPUT 1


// Set or clear a register bit
#define sbi(reg, pos) { reg |=  (1 << pos); }
#define cbi(reg, pos) { reg &= ~(1 << pos); }


// digitalWrite()
#define digitalWrite(signal, state) \
	_digitalWrite(PORT_##signal, PIN_##signal, state)

#define  _digitalWrite(port, pin, state)  __digitalWrite(port, pin, state)

#define __digitalWrite(port, pin, state) \
	{ if (state) sbi(PORT##port, pin) else cbi(PORT##port, pin); }


// digitalRead()
#define digitalRead(signal) \
	_digitalRead(PORT_##signal, PIN_##signal)

#define  _digitalRead(port, pin)  __digitalRead(port, pin)

#define __digitalRead(port, pin) ( PIN##port & (1 << pin) )


// pinMode()
#define pinMode(signal, state) \
	_pinMode(PORT_##signal, PIN_##signal, state)

#define  _pinMode(port, pin, state)  __pinMode(port, pin, state)

#define __pinMode(port, pin, state) \
	{ if (state) sbi(DDR##port, pin) else cbi(DDR##port, pin); }



#endif  /* !__PINDEFS_H__ */
