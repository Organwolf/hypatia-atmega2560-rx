/*
*  I2C_Client.c
* 
*  Drivers for ATmega2560 as I2C client.
*
*  Created: 2018-05-01 14:37:40
*  Author: SleepyOak, Filip Nilsson, Aron Polner
*/

#include <asf.h>
#include <util/twi.h>
#include "I2C_Client.h"
#include <stdio.h>
#include "uart.h"

#define F_CPU 16000000UL
#define nbrOfBytes 10		//When the master requests, the client returns 10bytes


volatile uint8_t TWI_Command = 1;
volatile uint8_t TWI_datatrack = 0;				//tracks how much data has been sent

/* 
*  This functions ---
*  describe the address 
*
*/
void I2C_Client_Init(uint8_t address){
	
	ioport_set_port_dir(IOPORT_PORTD, (1<<PD6), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_HIGH);
	
	ioport_set_port_dir(IOPORT_PORTD, (1<<PD0), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD0), IOPORT_PIN_LEVEL_HIGH);	//pull-up SCL

	ioport_set_port_dir(IOPORT_PORTD, (1<<PD1), IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PORTD, (1<<PD1), IOPORT_PIN_LEVEL_HIGH);	//pull-up SDA
	
	//Enable global Interrupts
	sei();
	//Set the client address				
	TWAR = (address<<1);	
	// Enable TWI and TWI interrupt flag
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWEA)|(1<<TWINT);
}

/*
*  This interrupt service routine ---
*
*
*/
ISR(TWI_vect){
	//UCSR1B |= ~(1<<RXCIE1); //Do we need to stop the USART1 interrupt... 
	// set pin to low if interrupt is called
	ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_LOW);
	
	if (TW_STATUS == TW_SR_STOP){		// stop command or repeated start, set slave to prepare to be addressed. if repeated start likely going to be addressed again but as transmitter
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);

		} else if (TW_STATUS == TW_ST_SLA_ACK) {	// if slave address comes up with read command (slave is transmitter)
		
		if (TWI_Command == 1){					// commands 1 (currently) are the commands that require transmission
			TWDR = pos[TWI_datatrack];
			TWI_datatrack++;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

			} else {								// if not one of these commands then the system has been commanded incorrectly, set to send 0xFF and receive nack
			TWDR = 0xFF;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE);
		}

		} else if (TW_STATUS == TW_ST_DATA_ACK){	// if there is still more transmitting to do
		TWDR = pos[TWI_datatrack];
		TWI_datatrack++;
		
		if (TWI_datatrack == nbrOfBytes) {// is this data byte the last one to be sent?, if yes prepare to receive nack
			TWI_datatrack = 0;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE);
			//UCSR1B |= (1<<RXCIE1); //Do we need to start the USART1 interrupt... 

			} else {
			TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
		}

		} else if (TW_STATUS == TW_ST_LAST_DATA) { // if last data has been sent and ack received when it shouldn't have, switch to not addressed
		TWCR = (1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

		} else {									// (command not recognized, state not accounted for), clear flag, set to prepare for next addressing
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	}
}