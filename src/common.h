/*
 * common.h
 *
 * Created: 2016-04-19 12:43:15
 *  Author: alex.rodzevski
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#ifndef F_CPU
/* 16 MHz clock speed, needs to be defined before including delay.h */
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

#define BAUD    2400		//9600

/* TODO: Define RTC 7-bit slave address */
#define DS1307                  (uint8_t)0x68

/* TODO: Define EEPROM 7-bit slave address */
#define AT24C32                 (uint8_t)0x50

/* Un-comment to activate the ADC-EEPROM Reference Application */
//#define APP_ADC_EEPROM



/************************************************************************/
/* Author Philip Ekholm                                                                     */
/************************************************************************/

/*
 * This macro is used to set a specific bit in a register.
 */
#define SET_BIT(reg, pos)	(reg |= (1 << pos))

/*
 * This macro is used to clear a specific bit in a register.
 */
#define CLR_BIT(reg, pos)   (reg &= ~(1 << pos))

/*
 * This macro is used to modify several bits of a register.
 * Example:
 *	Bit 7-4 of PORTD should be set to 1010, while the
 *	rest of the register should NOT be modified! Usage:
 *		SET_BIT_LEVELS(PORTD, 0b00001111, 0b10100000);
 * The bit mask is used to clear the bits that should be modified. The bit
 * mask also protects the bits that shouldn't be modified.
 */
#define SET_BIT_LEVELS(reg, bit_mask, bit_data) \
	(reg) = (((reg) & (bit_mask)) | ((bit_data) & ~(bit_mask)))

#endif /* COMMON_H_ */