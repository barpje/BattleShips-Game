#ifndef GAME_H
#define GAME_H
#include "Board.h"
#include "uart.h"
#include "Ship_utils.h"

volatile uint8_t whose_turn = 1;
volatile uint8_t game_over = 0;
volatile uint8_t myShips;
volatile uint8_t enemyShips;

struct Game{
    struct Board myBoard;
    struct Board enemyBoard;
};
void boardSettings(struct Game* game){
	game->myBoard.x = 0;
	game->myBoard.y = 20;
  board_init(20, 20, &game->myBoard);
  board_init(20, 20, &game->enemyBoard);
	printBoardLabels(&game->myBoard);
	draw_board(game->myBoard.fields);
}
void gameInit(struct Game* game){
	boardSettings(game);
  pickYourShips(game->myBoard.fields);
  myShips = 4 * 1 + 3 * 2 + 2 * 3 + 1 * 4;
  enemyShips = myShips;
  print_string("Dotknij, aby kontynuowac...", 0, 250, LCDWhite);
  tp_wait4touch();
	tp_wait4release();
  whose_turn = LPC_UART0->RBR == READY ? 0 : 1;
  clearInfoPanel(LCDBlack);

	if (whose_turn == 1){
		sendInfo(READY);
		print_string("Czekam na przeciwnika...", 0, 240, LCDWhite);
		waitForACK();
		clearInfoPanel(LCDBlack);
	}
	else{
		sendInfo(READY);
		clearInfoPanel(LCDBlack);
	}
}
int check_shoot(int x, int y, struct Field board[][BOARD_SIZE]){
	uint8_t info = 0;
	waitForACK();
	readInfo(&info);
	if (info == HITTED){
		board[x][y].type = HIT;
		drawField(&(board[x][y]));
		enemyShips -= 1;
		return enemyShips == 0 ? 1 : 0;
	}else if(board[x][y].type == HIT || board[x][y].type == MISS){
		return 1;
	}else{
		board[x][y].type = MISS;
		drawField(&(board[x][y]));//?
		return 1;
	}
}	
void shoot(struct Field board[][BOARD_SIZE]){
  int x, y;

	do {
		do {
			getTouch(&x, &y);
		} while (correctFieldIndex(&x, &y) != 0);
		
		sendInfo(READY);
		sendInfo(x + 1);
		sendInfo(y + 1);
	} while (check_shoot(x, y, board) == 0);
}

void receiveShoot(struct Board* board){
	uint8_t x, y;
	waitForACK();
	
	readInfo(&x);
	readInfo(&y);
	x -= 1;
	y -= 1;
	sendInfo(READY);

	if (board->fields[x][y].type == SHIP){ 
		board->fields[x][y].type = HIT;
		drawField(&(board->fields[x][y]));
    	myShips -= 1;
		sendInfo(HITTED);
		if(myShips == 0) return;
		else receiveShoot(board);
	}	else if(board->fields[x][y].type == HIT){
		sendInfo(MISSED);
	}else {
		board->fields[x][y].type = MISS;
		drawField(&(board->fields[x][y]));
		sendInfo(MISSED);
	}
}

void play(struct Game* game){
	
  if (whose_turn == 1) {
    draw_board(game->enemyBoard.fields);
		print_string("Strzelaj!", 0, 240, LCDWhite);
		shoot(game->enemyBoard.fields);

		if(enemyShips == 0) {
			clearInfoPanel(LCDBlack);
			print_string("Wygrales!", 0, 240, LCDWhite);
			game_over = 1;
		} else {
			draw_board(game->myBoard.fields);
			clearInfoPanel(LCDBlack);
		}		
	} else {
			print_string("Jestes pod ostrzalem!", 0, 240, LCDWhite);
    	draw_board(game->myBoard.fields);
			receiveShoot(&game->myBoard);

		if(myShips == 0) {
			clearInfoPanel(LCDBlack);
			print_string("Przegrales!", 0, 240, LCDWhite);
			game_over = 2;
		} else {
			draw_board(game->enemyBoard.fields);
			clearInfoPanel(LCDBlack);
		}	
	}
	whose_turn = !whose_turn;
}

bool isResetButtonTouched(uint8_t x, uint16_t y) {
	return (
		(x > 60 && x < 180) &&
		(y > 260 && y < 300)
	);
}

void reset(struct Game* game) {
	whose_turn = 1;
	game_over = 0;
}

void getUserChoice(uint8_t* playAgain, struct Game* game) {
	
	fill_rect(60, 260, 120, 40, LCDWhite);
	print_string("NOWA GRA", 80, 270, LCDRed);
	
	int  x, y;
	while(1){
		getTouch(&x, &y);

		if (isResetButtonTouched(x, y)) {
			reset(game);
			*playAgain = 1;
			break;
		}

	}
}

#endif