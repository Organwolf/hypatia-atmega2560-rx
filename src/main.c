/**
 *
 */
#include <asf.h>
#include "usart1.h"
#include <stdio.h>
#include "uart.h"
#include "recievedData.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sei();
	usart1_init();
	uart_init();

	uint8_t test = usart1_getChar();
	char str[20];
	sprintf(str, "val: %d",test);
	uart_write_str(str);
	
	while(1){
		//do nothing
	}

	/* Insert application code here, after the board has been initialized. */
}
