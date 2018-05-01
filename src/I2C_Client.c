/*
 * I2C_Client.c
 *
 * Created: 2018-05-01 14:37:40
 *  Author: SleepyOak, Filip Nilsson, Aron Polner
 */ 
#include <asf.h>
#include <util/twi.h>

#define F_CPU 16000000UL
#define nbrOfBytes 30

volatile uint8_t TWI_Command = 1;
volatile uint8_t TWI_datatrack = 0;				//tracks how much data has been sent
volatile uint8_t buffertx[nbrOfBytes] ={1,7,4,    //Avlämning X
										2,5,5,    //Avlämning Y
										1,8,0,    //Kub X
										3,7,4,    //Kub Y
										3,1,2,    //Kula X
										0,0,7,    //Kula Y
										3,6,7,    //Glas X
										4,0,0,    //Glas Y
										1,4,2,    //Robot X
										3,5,0};   //Robot Y


void I2C_Client_Init(uint8_t adress){
		/* Insert application code here, after the board has been initialized. */
		ioport_set_port_dir(IOPORT_PORTD, (1<<PD6), IOPORT_DIR_OUTPUT);
		ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_HIGH);
		
		ioport_set_port_dir(IOPORT_PORTD, (1<<PD0), IOPORT_DIR_OUTPUT);
		ioport_set_port_level(IOPORT_PORTD, (1<<PD0), IOPORT_PIN_LEVEL_HIGH);	//pull-up SCL

		ioport_set_port_dir(IOPORT_PORTD, (1<<PD1), IOPORT_DIR_OUTPUT);
		ioport_set_port_level(IOPORT_PORTD, (1<<PD1), IOPORT_PIN_LEVEL_HIGH);	//pull-up SDA
		
		sei();				//Enable global Interrupts
		TWAR = (adress<<1);	//Set the client adress
		// Enable TWI and TWI interrupt flag
		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWEA)|(1<<TWINT);
}
ISR(TWI_vect){
	// set pin to low if interrupt is called
	ioport_set_port_level(IOPORT_PORTD, (1<<PD6), IOPORT_PIN_LEVEL_LOW);
	// dummy variable to store data
	uint8_t data;
	
	if (TW_STATUS == TW_SR_STOP){		// stop command or repeated start, set slave to prepare to be addressed. if repeated start likely going to be addressed again but as transmitter
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);

		} else if (TW_STATUS == TW_ST_SLA_ACK) {	// if slave address comes up with read command (slave is transmitter)
		
		if (TWI_Command == 1){					// commands 1 (currently) are the commands that require transmission
			TWDR = buffertx[TWI_datatrack];
			TWI_datatrack++;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

			} else {								// if not one of these commands then the system has been commanded incorrectly, set to send 0xFF and receive nack
			TWDR = 0xFF;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE);
		}

		} else if (TW_STATUS == TW_ST_DATA_ACK){	// if there is still more transmitting to do
		TWDR = buffertx[TWI_datatrack];
		TWI_datatrack++;
		
		if (TWI_datatrack == nbrOfBytes) {// is this data byte the last one to be sent?, if yes prepare to receive nack
			TWI_datatrack = 0;
			TWCR = (1<<TWINT)|(0<<TWSTO)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE);

			} else {
			TWCR = (1<<TWINT)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
		}

		} else if (TW_STATUS == TW_ST_LAST_DATA) { // if last data has been sent and ack received when it shouldn't have, switch to not addressed
		TWCR = (1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);

		} else {									// (command not recognized, state not accounted for), clear flag, set to prepare for next addressing
		TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	}
}