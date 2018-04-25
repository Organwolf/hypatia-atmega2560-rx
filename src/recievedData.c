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
int checkBit [3] = {0};
uint8_t finalBits[4] = {0};
uint8_t correctBinary=0;
uint8_t recievedData(uint8_t dataReceived[]){
	
	int p1=0;
	int p2=0;
	int p3=0;
	
	checkBit[0]=dataReceived[1];
	checkBit[1]=dataReceived[2];
	checkBit[2]=dataReceived[4];
	
	p1 =(checkBit[0] + dataReceived[3] + dataReceived[5] + dataReceived[7]);
	p2 =(checkBit[1] + dataReceived[3] + dataReceived[6] + dataReceived[7]);
	p3 =(checkBit[2] + dataReceived[5] + dataReceived[6] + dataReceived[7]);

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

	if(checkBit[0]==0 && checkBit[1]==0 && checkBit[2]==0){
		toBinary(dataReceived);
		return correctBinary;
	}
	else if(checkBit[0]==1 && checkBit[1]==0 && checkBit[2]==0){            //bit 1
		//printf("Checkbit 1 is wrong \n");
		wrongBit=1;
		//uart_write_str("HEJJJJJJJJJJ");
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==1 && checkBit[2]==0){            // Bit 2
		//printf("Checkbit 2 is wrong \n");
		wrongBit=2;

		correctData(wrongBit, dataReceived);

	}
	else if(checkBit[0]==1 && checkBit[1]==1 && checkBit[2]==0){            //bit 3
		//printf("Databit 1 is wrong \n");
		wrongBit=3;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==0 && checkBit[2]==1){            // Bit 4
		//printf("CheckBit 3 is wrong \n");
		wrongBit=4;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==1 && checkBit[1]==0 && checkBit[2]==1){            // Bit 5
		//printf("Databit 2 is wrong \n");
		wrongBit=5;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==0 && checkBit[1]==1 && checkBit[2]==1){            //bit 6
		//printf("Databit 3 is wrong \n");
		wrongBit=6;
		correctData(wrongBit, dataReceived);
	}
	else if(checkBit[0]==1 && checkBit[1]==1 && checkBit[2]==1){            // Bit 7
		//printf("Databit 4 is wrong \n");
		wrongBit=7;
		correctData(wrongBit, dataReceived);
	}
	return correctBinary;
}

void correctData(int wrongBit, int dataReceived []){
	if(dataReceived[wrongBit]==1){
		dataReceived[wrongBit]=0;
	}
	else{
		dataReceived[wrongBit]=1;
	}
	toBinary(dataReceived);
}

void toBinary(uint8_t dataTosend []){
	
	finalBits[0]=dataTosend[3];			// data bit 1
	finalBits[1]=dataTosend[5];
	finalBits[2]=dataTosend[6];
	finalBits[3]=dataTosend[7];
	
	for(int i=0; i<4;i++){
		
		if(finalBits[i]==1){
			correctBinary|=(1<<i);
		}
	}
}















