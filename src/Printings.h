#ifndef PRINTINGS_H
#define PRINTINGS_H
#include "Drawings.h"
#include "asciiLib.h"

 void print_letter(int x, int y, unsigned char letter, uint16_t color){
	 unsigned char sign[16];
	 GetASCIICode(0, sign, letter);
	 for(int i = 0; i < 16; i++){
		 uint16_t row = sign[i];
		 for(int j = 0; j < 8; j++){
			 if(row & 1<<j)
				fill_pixel(x + 8 - j ,y + i, color);
		 }
	 }
 }
 
void print_string(const char* string, int x, int y, uint16_t color){
	int i = 0;
	while(string[i] != '\0'){
		print_letter(x + i * 8 , y, string[i], color);
		++i;
	}
}

#endif
