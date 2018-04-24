/*
*Author: Jesper Anderberg
*/

#ifndef RECIEVEDDATA_H_INCLUDED
#define RECIEVEDDATA_H_INCLUDED
uint8_t recievedData(uint8_t dataReceived[]);
void correctData(int wrongBit, int dataTosend []);
void toBinary(uint8_t dataTosend []);


#endif // RECIEVEDDATA_H_INCLUDED
