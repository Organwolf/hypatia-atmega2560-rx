/**
 *
 */
#include <avr/io.h>
#include <asf.h>
#include "usart1.h"
#include <stdio.h>
#include "uart.h"


uint8_t t = 0;
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sei();
	usart1_init();
	uart_init();
/*	DDRD |= (1<<PA4);*/
	while(1){
		//do nothing
	}

	/* Insert application code here, after the board has been initialized. */
}
