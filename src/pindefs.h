#ifndef __PINDEFS_H__
#define __PINDEFS_H__


/*
 * Common pinouts
*/

// Output to level shifter for !RESET from transistor
#define  PORT_RST      A
#define  PIN_RST       0

// Target VCC
#define  PORT_VCC      A
#define  PIN_VCC       1


/*
 * Pinouts for HV serial programming
*/

// Target Clock Input
#define  PORT_SCI      D
#define  PIN_SCI       1

// Target Data Output
#define  PORT_SDO      D
#define  PIN_SDO       0


// Target Instruction Input
#define  PORT_SII      B
#define  PIN_SII       1

// Target Data Input
#define  PORT_SDI      B
#define  PIN_SDI       0


/*
 * Pinouts for HV parallel programming
*/

// Data pin(s)
#define PORT_DATA      B


// Target Clock Input
#define  PORT_WR       D
#define  PIN_WR        1

// Target Data Output
#define  PORT_RDY      D
#define  PIN_RDY       0


// Output enable
#define PORT_OE        D
#define PIN_OE         3

// Byte Select 1
#define PORT_BS1       D
#define PIN_BS1        4

// XTAL Action Bit 0
#define PORT_XA0       D
#define PIN_XA0        5

// XTAL Action Bit 1
#define PORT_XA1       D
#define PIN_XA1        6


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
