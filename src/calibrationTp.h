#ifndef calib_h
#define calib_h
#include "Printings.h"
#include "TP_Open1768.h"

volatile int touched = 0;

typedef struct t_Coords{
	int x;
	int y;
} Coords;

volatile Coords currentTouch;

void tp_wait4touch(void){
	while (touched == 0);
}

void tp_wait4release(void){
	while (touched == 1);
}

typedef struct t_Matrix{
	int32_t An, Bn, Cn, Dn, En, Fn, k;
} Matrix;

volatile Matrix matrix;

void recalculateCoords(){
	int tempX = currentTouch.x;
	currentTouch.x = (currentTouch.x * matrix.An + currentTouch.y * matrix.Bn + matrix.Cn) / matrix.k;
	currentTouch.y = (tempX * matrix.Dn + currentTouch.y * matrix.En + matrix.Fn) / matrix.k;
}

int SetCalMatrix(const Coords *calibPoints, Coords *TPpoints, volatile Matrix *calculatedPoints){
	calculatedPoints->k = (
		(TPpoints[0].x - TPpoints[2].x) * (TPpoints[1].y - TPpoints[2].y)) - 
		((TPpoints[1].x - TPpoints[2].x) * (TPpoints[0].y - TPpoints[2].y)
	);

	if (calculatedPoints->k == 0)
		return 0;

	calculatedPoints->An = (
		(calibPoints[0].x - calibPoints[2].x) * (TPpoints[1].y - TPpoints[2].y)) -
		((calibPoints[1].x - calibPoints[2].x) * (TPpoints[0].y - TPpoints[2].y)
	);

	calculatedPoints->Bn = (
		(TPpoints[0].x - TPpoints[2].x) * (calibPoints[1].x - calibPoints[2].x)) -
		((calibPoints[0].x - calibPoints[2].x) * (TPpoints[1].x - TPpoints[2].x)
	);

	calculatedPoints->Cn = (
		(TPpoints[2].x * calibPoints[1].x - TPpoints[1].x * calibPoints[2].x) * TPpoints[0].y +
		(TPpoints[0].x * calibPoints[2].x - TPpoints[2].x * calibPoints[0].x) * TPpoints[1].y +
		(TPpoints[1].x * calibPoints[0].x - TPpoints[0].x * calibPoints[1].x) * TPpoints[2].y
	);

	calculatedPoints->Dn = (
		(calibPoints[0].y - calibPoints[2].y) * (TPpoints[1].y - TPpoints[2].y) -
		(calibPoints[1].y - calibPoints[2].y) * (TPpoints[0].y - TPpoints[2].y)
	);

	calculatedPoints->En = (
		(TPpoints[0].x - TPpoints[2].x) * (calibPoints[1].y - calibPoints[2].y) -
		(calibPoints[0].y - calibPoints[2].y) * (TPpoints[1].x - TPpoints[2].x)
	);

	calculatedPoints->Fn = (
		(TPpoints[2].x * calibPoints[1].y - TPpoints[1].x * calibPoints[2].y) * TPpoints[0].y +
		(TPpoints[0].x * calibPoints[2].y - TPpoints[2].x * calibPoints[0].y) * TPpoints[1].y +
		(TPpoints[1].x * calibPoints[0].y - TPpoints[0].x * calibPoints[1].y) * TPpoints[2].y
	);

	return 1;
}

void EINT3_IRQHandler(){
	touched = !touched;
	//NVIC_DisableIRQ(EINT3_IRQn);
	int count = 15;
	int sumX = 0;
	int sumY = 0;
	int x = 0;
	int y = 0;
	int i = 0;
	while (i < count){
		int deadTime = 10000;
		while(--deadTime);
		touchpanelGetXY(&y, &x);
		if (x == 0xFFF)
			continue;
		sumX += x;
		sumY += y;
		i++;
	}
	currentTouch.y = sumX / count;
	currentTouch.x = sumY / count;
  
	LPC_GPIOINT->IO0IntClr = 1 << 19;
	//NVIC_EnableIRQ(EINT3_IRQn);
	
}

#define CROSS_OFFSET 45
static const Coords CalPoints[3] = {
	{CROSS_OFFSET, CROSS_OFFSET},
	{CROSS_OFFSET, LCD_MAX_Y - CROSS_OFFSET},
	{LCD_MAX_X - CROSS_OFFSET, LCD_MAX_Y - CROSS_OFFSET}};

void runTouchPanelCalibration(void){
	Coords TPsamp[3];

	do{
		for (uint32_t i = 0; i < 3; i++){
			drawCross(CalPoints[i].x, CalPoints[i].y);
			tp_wait4touch();
			TPsamp[i] = currentTouch;
			tp_wait4release();
		}
	} while (!SetCalMatrix(CalPoints, TPsamp, &matrix));
}

void getTouch(int *x, int *y){
	tp_wait4touch();
	recalculateCoords();
	*x = currentTouch.x;
	*y = currentTouch.y;
	tp_wait4release();
}

#endif
