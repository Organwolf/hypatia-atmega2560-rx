/*
* Hamming decoder.
*
* Author Jesper Anderberg
*/

#include <stdio.h>
#include <stdlib.h>
#include "recievedData.h"
#include "uart.h"


int wrongBit;
int checkBit [3];
uint8_t finalBits[4];
uint8_t correctBinary;
uint8_t recievedData(uint8_t dataReceived[]){
	finalBits[0] = 0;
	finalBits[1] = 0;
	finalBits[2] = 0;
	finalBits[3] = 0;
	correctBinary = 0;
	
	dataReceived[7]=0;			// Make sure that the last bit is set to zero
	int p1=0;
	int p2=0;
	int p3=0;
	
// 		char str[20];
// 	    sprintf(str, "Check0: %d\n",dataReceived[0]);
// 		uart_write_str(str);
// 		sprintf(str, "Check1: %d\n",dataReceived[1]);
// 		uart_write_str(str);
// 	 	sprintf(str, "Check2: %d\n",dataReceived[3]);
// 	 	uart_write_str(str);

	
	checkBit[0]=dataReceived[0];
	checkBit[1]=dataReceived[1];
	checkBit[2]=dataReceived[3];
	
	p1 =(checkBit[0] + dataReceived[2] + dataReceived[4] + dataReceived[6]);
	p2 =(checkBit[1] + dataReceived[2] + dataReceived[5] + dataReceived[6]);
	p3 =(checkBit[2] + dataReceived[4] + dataReceived[5] + dataReceived[6]);

	if(p1%2==0){
		checkBit[0]=0;
	}
	else{
		checkBit[0]=1;
	}
	if(p2%2==0){
		checkBit[1]=0;
	}
	else{
		checkBit[1]=1;
	}

	if(p3%2==0){
		checkBit[2]=0;
	}
	else{
		checkBit[2]=1;
	}
	// 	 	char str[20];
	// 	 	sprintf(str, "Check0: %d\n",checkBit[0]);
	// 	 	uart_write_str(str);
	// 	 	sprintf(str, "Check1: %d\n",checkBit[1]);
	//      	uart_write_str(str);
	// 	 	sprintf(str, "Check2: %d\n",checkBit[3]);
	// 	 	uart_write_str(str);

	if(checkBit[0]==0 && checkBit[1]==0 && checkBit[2]==0){
		toBinary(dataReceived);
		//	char str[20];
		//	sprintf(str, "correctBinary: %d\n",correctBinary);
		//uart_write_str(str);
		return correctBinary;
	}
	else if(checkBit[0]==1 && checkBit[1]==0 && checkBit[2]==0){            //bit 1
		//printf("Checkbit 1 is wrong \n");
		wrongBit=0;
		//uart_write_str("HEJJJJJJJJJJ");
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==1 && checkBit[2]==0){            // Bit 2
		//printf("Checkbit 2 is wrong \n");
		wrongBit=1;

		correctData(wrongBit, dataReceived);

	}
	else if(checkBit[0]==1 && checkBit[1]==1 && checkBit[2]==0){            //bit 3
		//printf("Databit 1 is wrong \n");
		wrongBit=2;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==0 && checkBit[2]==1){            // Bit 4
		//printf("CheckBit 3 is wrong \n");
		wrongBit=3;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==1 && checkBit[1]==0 && checkBit[2]==1){            // Bit 5
		//printf("Databit 2 is wrong \n");
		wrongBit=4;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==1 && checkBit[2]==1){            //bit 6
		//printf("Databit 3 is wrong \n");
		wrongBit=5;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==1 && checkBit[1]==1 && checkBit[2]==1){            // Bit 7
		//printf("Databit 4 is wrong \n");
		wrongBit=6;
		correctData(wrongBit, dataReceived);
	}
	return correctBinary;
}

void correctData(int wrongBit, int dataTosend []){
	int i;
	for(i=0; i<8;i++){
		if(dataTosend[wrongBit]==1){
			dataTosend[wrongBit]=0;
			i=8;
		}
		else if(dataTosend[wrongBit] == 0){
			dataTosend[wrongBit]=1;
			i=8;
		}
	}
	toBinary(dataTosend);
}

void toBinary(uint8_t dataTosend []){
	
	finalBits[0]=dataTosend[2];  //6
	finalBits[1]=dataTosend[4];  //5
	finalBits[2]=dataTosend[5];  //4
	finalBits[3]=dataTosend[6];  //2
	
	for(int i=0; i<4;i++){
		
		if(finalBits[i]==1){
			correctBinary|=(1<<i);
		}
	}
}















