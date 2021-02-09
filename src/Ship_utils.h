#ifndef SHIPUTILS_H
#define SHIPUTILS_H

#include <stdlib.h>
#include <stdbool.h>

#include "Board.h"
#include "Field.h"
#include "Printings.h"
#include "uart.h"
#include "calibrationTp.h"


void swap(uint8_t *a, uint8_t *b){
	uint8_t tmp = *a;
	*a = *b;
	*b = tmp;
}

bool isOutsideTheBoard(int x, int y) {
	return (
		x < 0 || 
		x > BOARD_SIZE - 1 || 
		y < 0 || 
		y > BOARD_SIZE - 1
	);
}

bool isPositioningAndLengthCorrect(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t length) {
	return (startX == endX && abs(startY - endY) == length - 1) || (startY == endY && abs(startX - endX) == length - 1);
}

bool isAreaAvailable(struct Field board[][BOARD_SIZE], uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) {
	for (int i = startX - 1; i <= endX + 1; i++) {
		for (int j = startY - 1; j <= endY + 1; j++) {
			if (isOutsideTheBoard(i, j)) continue;
			if (board[i][j].type == SHIP) {
				return false;
			}
		}
	}

	return true;
}

void placeShip(struct Field board[][BOARD_SIZE], uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) {
	for (uint8_t i = startX; i <= endX; i++){
		for (uint8_t j = startY; j <= endY; j++){
			board[i][j].type = SHIP;
			drawField(&board[i][j]);
		}
	}
}


int tryToPlaceShip(struct Field board[][BOARD_SIZE], uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t length){
	if (!isPositioningAndLengthCorrect(startX, startY, endX, endY, length))
		return 1;

	if (startX > endX) swap(&startX, &endX);
	if (startY > endY) swap(&startY, &endY);

	if (!isAreaAvailable(board, startX, startY, endX, endY))
		return 1;

	placeShip(board, startX, startY, endX, endY);
	return 0;
}

void setAvailableFileds(struct Field board[][BOARD_SIZE], uint8_t startX, uint8_t startY, uint8_t length1, enum FieldType mode) {
	uint8_t length = length1 - 1;
	if (!isOutsideTheBoard(startX + length, startY) && board[startX + length][startY].type != SHIP) {
		board[startX + length][startY].type = mode;
		drawField(&board[startX + length][startY]);
	}

	if (!isOutsideTheBoard(startX - length, startY) && board[startX - length][startY].type != SHIP) {
		board[startX - length][startY].type = mode;
		drawField(&board[startX - length][startY]);
	}

	if (!isOutsideTheBoard(startX, startY + length) && board[startX][startY + length].type != SHIP) {
		board[startX][startY + length].type = mode;
		drawField(&board[startX][startY + length]);
	}

	if (!isOutsideTheBoard(startX, startY - length) && board[startX][startY - length].type != SHIP) {
		board[startX][startY - length].type = mode;
		drawField(&board[startX][startY - length]);
	}
}


void getShipCoordinates(struct Field board[][BOARD_SIZE], uint8_t length) {
	int startX, startY, endX, endY;

  clearInfoPanel(LCDBlack);
	print_string("Tryb wybierania!!!", 0, 240, LCDWhite);
	print_string("Umiesc statek ", 0, 290, LCDWhite);
	print_letter( 110, 290, length + '0', LCDWhite);
	print_string(" masztowy!", 120, 290, LCDWhite);

	do {
		do {
			getTouch(&startX, &startY);
		} while (correctFieldIndex(&startX, &startY) != 0 || board[startX][startY].type == SHIP);

		board[startX][startY].type = SHIP_PICK_START;
		drawField(&board[startX][startY]);

		if (length != 1) {

			setAvailableFileds(board, startX, startY, length, AVAILABLE_SHIP_END);

      do {
        getTouch(&endX, &endY);
      } while (correctFieldIndex(&endX, &endY) != 0  || board[startX][startY].type == SHIP);		

			setAvailableFileds(board, startX, startY, length, EMPTY);		
    } else {
      endX = startX;
      endY = startY;
    }		

		board[startX][startY].type = EMPTY;
		drawField(&board[startX][startY]);

	} while (tryToPlaceShip(board, startX, startY, endX, endY, length) != 0);
	clearInfoPanel(LCDBlack);
}

struct ShipTypeInfo {
	uint8_t quantity;
	uint8_t length;
};

struct ShipTypeInfo shipsMetaInfos[] = {{1, 4}, {2, 3}, {3, 2}, {4, 1}};


void pickYourShips(struct Field board[][BOARD_SIZE]){
	uint8_t numberOfShipTypes = sizeof(shipsMetaInfos)/sizeof(shipsMetaInfos[0]);

	for (uint8_t i = 0; i < numberOfShipTypes; i++) {
    struct ShipTypeInfo* ship = &shipsMetaInfos[i];
		
		for (uint8_t j = 0; j < ship->quantity; j++) {
			getShipCoordinates(board, ship->length);
		}
  }
}

#endif