#ifndef __OBSTACLE__ 																																	
#define __OBSTACLE__	

#include <allegro5/allegro5.h>		
#include <allegro5/allegro_primitives.h>

#include "pistol.h"
#include "enemy.h"
#include "ship.h"

#define OBSTACLE_LIVES 10
#define OBSTACLE_ROW 3
#define OBSTACLE_COLUMN 4																																																														//Quantidade de pixels que um quadrado se move por passo
																														
typedef struct {																																																																
	unsigned short x;																																
	unsigned short y;	
	unsigned short height;
	unsigned short width;	
	unsigned short lifeCount;																																																													
} obstacle;			


obstacle** obstacle_create(unsigned short x, unsigned short y, unsigned short height, unsigned short width, int max_x, int max_y);																											
void draw_obstacle(ALLEGRO_BITMAP* SquareCorner, ship *ship, EnemyBoard *enemyBoard,obstacle **obstacleMatrixUm,obstacle **obstacleMatrixDois,obstacle **obstacleMatrixTres,obstacle **obstacleMatrixQuatro,
ALLEGRO_BITMAP* DestroyedObstacleOne, ALLEGRO_BITMAP* DestroyedObstacleTwo, ALLEGRO_BITMAP* DestroyedObstacleTree, ALLEGRO_BITMAP* DestroyedObstacleFour, ALLEGRO_BITMAP* DestroyedObstacleFive, 
ALLEGRO_BITMAP* DestroyedObstacleSix, ALLEGRO_BITMAP* DestroyedObstacleSeven, ALLEGRO_BITMAP* DestroyedObstacleEight,ALLEGRO_BITMAP* DestroyedObstacleNine,ALLEGRO_BITMAP* DestroyedCornerOne, 
ALLEGRO_BITMAP* DestroyedCornerTwo, ALLEGRO_BITMAP* DestroyedCornerTree);
void obstacle_collision(pistol *shotsList, obstacle **obstacleMatrix, int SquareCornerWidth, int SquareCornerHeight);
void obstacle_destroy(obstacle ***obstacleMatrix);		

#endif		