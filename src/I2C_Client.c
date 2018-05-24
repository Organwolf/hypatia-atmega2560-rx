/*
*  I2C_Client.c
* 
*  Drivers for ATmega2560 as I2C client.
*
*  Created: 2018-05-01 14:37:40
*  Author: SleepyOak, Filip Nilsson, Aron Polner.
*/

#include <asf.h>
#include <util/twi.h>
#include "I2C_Client.h"
#include <stdio.h>
#include "uart.h"

#define F_CPU 16000000UL
#define nbrOfBytes 10							// the number of Bytes per communication with the master I2C device


volatile uint8_t TWI_Command = 1;
volatile uint8_t TWI_datatrack = 0;				// tracks how much data has been sent

/************************************************************************/
/* The function I2C_Client_Init() takes an uint8_t address and	        */
/* Enables interrupt T1 (PD6) as well as SCL (PD0) and SDA (PD1).		*/
/*																		*/
/* Further the global interrupt is enabled, the address loaded in to	*/
/* TWAR (the address register) and lastly TWI is enables along with		*/ 
/* the TWI interrupt flag.												*/
/************************************************************************/
void I2C_Client_Init(uint8_t address){
	
	ioport_set_port_dir(IOPORT_PORTD, (1<<PD6), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_HIGH);
	
	ioport_set_port_dir(IOPORT_PORTD, (1<<PD0), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD0), IOPORT_PIN_LEVEL_HIGH);	//pull-up SCL

	ioport_set_port_dir(IOPORT_PORTD, (1<<PD1), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD1), IOPORT_PIN_LEVEL_HIGH);	//pull-up SDA

	sei();
				
	TWAR = (address<<1);	
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWEA)|(1<<TWINT);
}

/************************************************************************/
/* This interrupt service routine handles the TWI communication         */
/*																		*/
/* The client (ATmega2560) is first prepared to be addressed by the 	*/
/* master device. Thereafter if the slave address is followed by a read */
/* command the client sends one byte of data from the pos[] buffer.		*/
/* If the byte sent is ACKED by the master this continues until 10		*/
/* bytes have been sent and NACKED.									    */
/*																		*/
/* If anything else should happen the TWCR (control register) is		*/
/* cleared and the I2C client is once again prepared for a new			*/
/* addressing.															*/
/************************************************************************/
ISR(TWI_vect){

	ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_LOW);
	
	if (TW_STATUS == TW_SR_STOP){
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
		
	} else if (TW_STATUS == TW_ST_SLA_ACK){	
		TWDR = pos[TWI_datatrack];
		TWI_datatrack++;
		TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

	} else if (TW_STATUS == TW_ST_DATA_ACK){
		TWDR = pos[TWI_datatrack];
		TWI_datatrack++;
		
		if (TWI_datatrack == nbrOfBytes){
			TWI_datatrack = 0;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE); 

		} else{
			TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
		}

	} else if (TW_STATUS == TW_ST_LAST_DATA){
		TWCR = (1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

	} else {									
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	}
}