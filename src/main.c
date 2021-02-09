#include <stdlib.h>
#include <stdbool.h>

#include "LPC17xx.h"                    // Device header
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"
#include "Open1768_LCD.h"
#include "asciiLib.h"

#include "Board.h"
#include "Game.h"
#include "uart.h"
#include "Ship_utils.h"
#include "calibrationTp.h"

void initPeripherals(void);

struct Game game;


int main(void) {
	initPeripherals();
	runTouchPanelCalibration();

	uint8_t playAgain = 0;

	do {
		gameInit(&game);

		while (game_over == 0) {
			play(&game);
		}

		getUserChoice(&playAgain, &game);
	} while (playAgain);
}


void initPeripherals(void) {
	lcdConfiguration();
	init_ILI9325();
	touchpanelInit();
	initBackground();
	UART_config();

	LPC_GPIOINT->IO0IntEnF = 1 << 19;
	NVIC_EnableIRQ(EINT3_IRQn);
}