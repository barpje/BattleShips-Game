#ifndef BOARD_H
#define BOARD_H
#include "Drawings.h"
#include "Printings.h"
#include "Field.h"
#include "uart.h"
#include <stdbool.h>

#define BOARD_SIZE 10
#define LINE_SIZE 2
#define FIELD_SIZE 20

struct Board{
	struct Field fields[10][10];
	int x;
	int y;
	int size;
};
// Function printing labels for game board
char horizonalLabels[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
char verticalLabels[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int letterTopPadding = 2;
int letterLeftPadding = 6;

void printBoardLabels(struct Board* board) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		int letterXOffset = board->x + i * FIELD_SIZE + i * LINE_SIZE;
		int letterYOffset = board->y + i * FIELD_SIZE + i * LINE_SIZE;

		print_letter(0 + letterLeftPadding, letterYOffset + letterTopPadding, verticalLabels[i], LCDYellow);
		print_letter(letterXOffset + letterLeftPadding, 0 + letterTopPadding, horizonalLabels[i], LCDYellow);
	}
}

void board_init(int x, int y, struct Board* board){
	board->x = x;
	board->y = y;
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			board->fields[i][j].y = y + j * FIELD_SIZE + j * LINE_SIZE;
			board->fields[i][j].x = x + i * FIELD_SIZE + i * LINE_SIZE;
			board->fields[i][j].type = EMPTY;
		}
	}
}
void draw_board(struct Field board[][BOARD_SIZE]){
	
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			struct Field current = board[i][j];
			switch(current.type){
				case EMPTY: 
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDBlueSea);
					break;
				case SHIP:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDGrey);
					break;
				case HIT:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDRed);
					break;
				case MISS:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDGreen);
					break;
				case SUNKEN:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDBlack);
					break;
				case SHIP_PICK_START:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDYellow);
					break;
				case AVAILABLE_SHIP_END:
					fill_rect(current.x, current.y, FIELD_SIZE, FIELD_SIZE, LCDWhite);
					break;
				default:
					break;
			}
		}
	}
}

void drawField(struct Field* current){
	switch(current->type){
				case EMPTY: 
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDBlueSea);
					break;
				case SHIP:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDGrey);
					break;
				case HIT:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDRed);
					break;
				case MISS:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDGreen);
					break;
				case SUNKEN:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDBlack);
					break;
				case SHIP_PICK_START:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDYellow);
					break;
				case AVAILABLE_SHIP_END:
					fill_rect(current->x, current->y, FIELD_SIZE, FIELD_SIZE, LCDWhite);
					break;
				default:
					break;
			}
}

int correctFieldIndex(int *x, int *y){
	*x -= 20;
	*x /= (FIELD_SIZE + LINE_SIZE);
	*y -= 20;
	*y /= (FIELD_SIZE + LINE_SIZE);
	if (*x < 0 || *x >= BOARD_SIZE || *y < 0 || *y >= BOARD_SIZE){
		return -1;
	}
	return 0;
}			
#endif
