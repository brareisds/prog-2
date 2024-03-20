#ifndef __SHIP__ 																																	
#define __SHIP__																																																																//Quantidade de pixels que um quadrado se move por passo

#include "Joystick.h"
#include "pistol.h"

#define SHIP_STEP 10	
#define INITIAL_LIVES 3																																

typedef struct {																																																																
	unsigned char face;																																
	unsigned short x;																																
	unsigned short y;	
	int height;
	int width;	
	int score;	
	unsigned short lifeCount;
	int destroyedShipTimer;																														
	joystick *control;																																
	pistol *gun;																																
} ship;																																			

ship* ship_create(int ShipImageHeight, int ShipImageWidth, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);	
void ship_move(ship *element, char steps, unsigned char trajectory, unsigned short max_x);
void ship_shot(ship *element);
void ship_destroy(ship *element);																												

#endif																																				