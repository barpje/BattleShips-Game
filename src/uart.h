#ifndef UART_H
#define UART_H
#include "TP_Open1768.h"
#include "PIN_LPC17xx.h"
#include <string.h>

#define READY 'r'
#define HITTED 'g'
#define MISSED 'b'

void sendInfo(const uint8_t info){
	while (!(LPC_UART0->LSR & (1 << 5)))
		;
	LPC_UART0->THR = info;
}

void waitForACK(){
	while (LPC_UART0->RBR != READY);
}

void readInfo(uint8_t *info){
	do{
		*info = LPC_UART0->RBR;
	} while (*info == 0);
}

void UART_config(){
	PIN_Configure(0, 2, 01, 0, 0);
	PIN_Configure(0, 3, 01, 1, 0);
	LPC_UART0->LCR = (1 << 7) | (3 << 0);
	LPC_UART0->FDR = (15 << 4) | (1 << 0);
	LPC_UART0->DLL = 13;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR = (3 << 0);
}

#endif