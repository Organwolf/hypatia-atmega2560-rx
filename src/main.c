/**
 *	Author: Filip Nilsson and Aron Polner
 */
#include <asf.h>
#include "usart1.h"
#include <stdio.h>
#include "uart.h"
#include "I2C_Client.h"

uint8_t SlaveAddress = 0x08;		//0x10

int main (void)
{
	I2C_Client_Init(SlaveAddress);
	board_init();
	sei();
	usart1_init();
	uart_init();
	
	while(1){
		//do nothing
	}
}
