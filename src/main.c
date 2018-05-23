/**
 * Created: 2018-05-20
 * Authors: Aron Polner & Filip Nilsson
 *
 * Main file for the Arduino Mega which is a part the indoor positioning system.
 * The Arduino Mega receives a predetermined structure of a data paket.
 * The Arduino Mega is able to identify the actual data within this data paket (see usart1.c).
 * 
 * The Arduino Mega also acts as a client in a TWI network. 
 * When the master requests data, the Arduino Mega responds with a data structure (see I2C_Client.c).
 */
#include <asf.h>
#include "usart1.h"
#include <stdio.h>
#include "uart.h"
#include "I2C_Client.h"

uint8_t clientAddress = 0x10;		//Master requests from this address.


/************************************************************************/
/* Main function.                                                       */
/* Contains different initializations.									*/
/************************************************************************/
int main (void)
{
	I2C_Client_Init(clientAddress);
	board_init();
	sei();
	usart1_init();
	uart_init();
	
	while(1){
		//do nothing
	}
}
