/*
 * File name: usart1.h
 *
 * Description: Header file for the driver implemented in usart1.c
 *
 * Created: 2018-05-20
 * Author: Filip Nilsson
 */ 

#ifndef UART_H_
#define UART_H_

void usart1_init(void);
void usart1_transmit(unsigned char data);
char usart1_getChar(void);
/************************************************************************/
/* Find the majority element within an array                            */
/************************************************************************/ 
uint8_t findMajority(volatile uint8_t arr[], volatile uint8_t n);

#endif /* UART_H_ */