#ifndef DRAWINGS_H
#define DRAWINGS_H
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"

void fill_rect(int x, int y, int w, int h, uint16_t color){
	lcdWriteReg(ADRX_RAM, x);
	lcdWriteReg(ADRY_RAM, y);
	lcdWriteReg(DATA_RAM, color);
	int localY = y;
	for(int i = 1; i < w * h + 1; i++){
			if((i - 1) % w == 0 && i != 1){
				lcdWriteReg(ADRY_RAM, ++localY);
				lcdWriteReg(ADRX_RAM, x);
				lcdWriteReg(DATA_RAM, color);
			}
			lcdWriteData(color);
	}
}
void clearInfoPanel(uint16_t color){
    fill_rect(0, 240, LCD_MAX_X, LCD_MAX_Y - 240, color);
}
void initBackground(){         
    fill_rect(0, 0, LCD_MAX_X, LCD_MAX_Y, LCDBlack);
}

void fill_pixel(const int x, const int y, uint16_t color){
	lcdWriteReg(ADRX_RAM, x);
	lcdWriteReg(ADRY_RAM, y);
	lcdWriteReg(DATA_RAM, color);
}

void line(const int x1, const int y1, const int x2, const int y2, uint16_t color){
     // zmienne pomocnicze
     int d, dx, dy, ai, bi, xi, yi;
     int x = x1, y = y1;
     // ustalenie kierunku rysowania
     if (x1 < x2)
     {
         xi = 1;
         dx = x2 - x1;
     }
     else
     {
         xi = -1;
         dx = x1 - x2;
     }
     // ustalenie kierunku rysowania
     if (y1 < y2)
     {
         yi = 1;
         dy = y2 - y1;
     }
     else
     {
         yi = -1;
         dy = y1 - y2;
     }
     // pierwszy piksel
     fill_pixel(x, y, color);
     // os wiodaca OX
     if (dx > dy)
     {
         ai = (dy - dx) * 2;
         bi = dy * 2;
         d = bi - dx;
         // petla po kolejnych x
         while (x != x2)
         {
             // test wsp�lczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 x += xi;
             }
             fill_pixel(x, y, color);
         }
     }
     // os wiodaca OY
     else{
         ai = ( dx - dy ) * 2;
         bi = dx * 2;
         d = bi - dy;
         // petla po kolejnych y
         while (y != y2){
             // test wsp�lczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 y += yi;
             }
             fill_pixel(x, y, color);
         }
     }
 }
void drawCross(uint16_t x, uint16_t y){
	line(x - 15, y, x - 2, y, 0xffff);
	line(x + 2, y, x + 15, y, 0xffff);
	line(x, y - 15, x, y - 2, 0xffff);
	line(x, y + 2, x, y + 15, 0xffff);
}


#endif
