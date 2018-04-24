/**
 *
 */
#include <asf.h>
#include "usart1.h"
#include <stdio.h>
#include "uart.h"
#include "recievedData.h"

uint8_t t = 0;
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sei();
	usart1_init();
	uart_init();
	
	while(1){
		//do nothing
	}

	/* Insert application code here, after the board has been initialized. */
}
