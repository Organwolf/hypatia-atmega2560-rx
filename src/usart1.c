/*
 * usart1.c
 *
 * For USART communication the TX1 is used. 
 * 2400 Baudrate, 8-bit
 * Predetermined data packets, see below. 
 *
 * Created: 2018-05-20
 * Author: Filip Nilsson
 * 
 */ 

#include <avr/io.h>
#include <asf.h>
#include <stdio.h>
#include "../common.h"
#include "usart1.h"
#include <avr/interrupt.h>
#include "uart.h"

#define MYUBRR  (unsigned int)(F_CPU/16/BAUD-1)		//Value to be put in UBBR
#define SYNC 0b11110101								//Dec: 245, Safe value for identifying a new packet
#define MY_PIN    IOPORT_CREATE_PIN(PORTB, 7)		//Onboard LED, used for debugging

extern volatile uint8_t pos[10] = {150,200,50,150,100,100,150,50,0,0};	//Index 0-7: predetermined. 8-9 coordinates for robot.
volatile uint8_t rec;
volatile uint16_t nbrOfTransmits = 0;
volatile uint8_t theIndex = 0;
volatile uint8_t byteCounter=0;
volatile uint8_t xSamples[10] = {0};
volatile uint8_t ySamples[10] = {0};
volatile uint8_t majorityX = 0;
volatile uint8_t majorityY = 0;
volatile uint8_t c = 0;

static int usart1_putchar(char c, FILE *unused)
{
	if (c == '\n')
	usart1_putchar('\r', 0);
	usart1_transmit(c);
	return 0;
}

FILE mystdout = FDEV_SETUP_STREAM(usart1_putchar, NULL, _FDEV_SETUP_WRITE);

/************************************************************************/
/* Initialize the USART1.                                               */
/************************************************************************/
void usart1_init(void)
{
	/* Set baud rate */
	UBRR1H = (unsigned char)(MYUBRR >> 8);
	UBRR1L = (unsigned char)MYUBRR;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Enable interrupt */
	UCSR1B |= (1<<RXCIE1);
	/* Set frame format: Async, No parity, 1 stop bit, 8 data */
	UCSR1C = (3<<UCSZ00);
	/* Re-rout stdout (printf) to use internal uart_putchar */
	stdout = &mystdout;
	ioport_init();
	
	ioport_set_pin_dir(MY_PIN, IOPORT_DIR_OUTPUT);
}

/************************************************************************/
/* Used to transmit a new Byte through USART1.                          */
/************************************************************************/
void usart1_transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!( UCSR1A & (1<<UDRE1)));
	/* Start transmission by loading data into the buffer */
	UDR1 = data;
}

/************************************************************************/
/* Used to receive a new Byte on the USART1.                            */
/************************************************************************/

char usart1_getChar(void){
	while (!( UCSR1A & (1<<RXC1)));
	return UDR1;
}

/************************************************************************************/
/* Used to determine if there is a majority element present within a certain array. */
/* If a majority is present, the value of it is returned.							*/
/* Otherwise the value 255 is returned, indicating that no majority is present      */
/************************************************************************************/
uint8_t findMajority(volatile uint8_t arr[], volatile uint8_t n)
{
	int maxCount = 0;
	int index = -1;
	for(int i = 0; i < n; i++)
	{
		int count = 0;
		for(int j = 0; j < n; j++)
		{
			if(arr[i] == arr[j])
			count++;
		}
		if(count > maxCount)//new maxCount found
		{
			maxCount = count;
			index = i;
		}
	}
	if (maxCount > n/2){	//Majority found!
		return arr[index];
	}
	else{					//No majority found
		return 255;			//"Safe" value, outside of gamearea
	}
}

/************************************************************************/
/* USART1 interrupt routine.											*/
/* Data packet: |theSync|+|10 times X|+|10 times Y|						*/                                                                    
/************************************************************************/
ISR(USART1_RX_vect)
{
	char str[20];
	rec = usart1_getChar();
	if((rec==SYNC)){
		theIndex=0;
		byteCounter=0;
		majorityX=findMajority(xSamples,10);
		majorityY=findMajority(ySamples,10);
		if((majorityX>=0 && majorityX<=200) && (majorityY>=0 && majorityY<=200)){	//Game area
		//if ((majorityX!=245) && (majorityY!=245)){		//Majority found?
			pos[8]=majorityX;	//byteindex for robot X
			pos[9]=majorityY;	//byteindex for robot Y
			ioport_set_pin_level(MY_PIN, 1);	//Show the a new majority has been recieved.
			//sprintf(str,"x: %d y: %d",(uint16_t)(pos[8]*2),(uint16_t)(pos[9]*2));
			//sprintf(str,"%d %d %d %d %d %d %d %d %d %d",pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7],pos[8],pos[9]);
			//uart_write_str(str);
		}
		//}
	}
	else if(theIndex==0){
		xSamples[byteCounter++]=rec;
		if(byteCounter==10){
			theIndex++;
			byteCounter=0;
		}
	}
	else{
		ySamples[byteCounter++]=rec;
	}
 	c++;
	if(c==2){
		ioport_set_pin_level(MY_PIN, 0);
		c=0;
	}
}