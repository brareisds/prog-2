#include <stdlib.h>
#include <stdio.h>

#include "ship.h"

ship* ship_create(int ShipImageHeight, int ShipImageWidth, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){			

	if ((x - ShipImageWidth/2 < 0) || (x + ShipImageWidth/2 > max_x) || (y - ShipImageWidth/2 < 0) || (y + ShipImageWidth/2 > max_y)) return NULL;												
																												
	ship *new_ship = (ship*) malloc(sizeof(ship));																								
	if (!new_ship) return NULL;	

	new_ship->width = ShipImageWidth;
	new_ship->height = ShipImageHeight;																															
	new_ship->face = face;																															
	new_ship->x = x;																																	
	new_ship->y = y;	
	new_ship->score = 0;
	new_ship->destroyedShipTimer = 0;
	new_ship->lifeCount = INITIAL_LIVES;																																
	new_ship->control = joystick_create();																											
	new_ship->gun = pistol_create();

	return new_ship;																																	
}

void ship_move(ship *element, char steps, unsigned char trajectory, unsigned short max_x){									
    
    /* Verifica se eh possivel mover a nave para a esquerda e se for, executa a movimentação */
	if (!trajectory){
        if ((element->x - steps * SHIP_STEP) >= 0) 
			element->x = element->x - steps * SHIP_STEP;
	}
	/* Verifica se eh possivel mover a nave para a direita e se for, executa a movimentação */
	else if (trajectory == 1){
        if ((element->x + steps * SHIP_STEP) + element->width <= max_x) 
			element->x = element->x + steps * SHIP_STEP;		
	}
}

void ship_shot(ship *element){																														
	bullet *shot;
	if (element->face == 2) shot = pistol_shot(element->x + element->width/2, element->y, element->face, element->gun);										
	if (shot) element->gun->shots = shot;
}

void ship_destroy(ship *element)
{																														
	pistol_destroy(element->gun);																														
	joystick_destroy(element->control);																											
	free(element);																																		
}