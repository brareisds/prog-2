#ifndef __ENEMY__
#define __ENEMY__

#define ENEMY_ROWS 5
#define ENEMY_COLUMNS 11
#define MAX_EXPLOSIONS ENEMY_ROWS * ENEMY_COLUMNS
#define ESPACAMENTO_ENTRE_ALIENS 24
#define MOTHER_SHIP_MOVE 10
#define MOTHER_SHIP_COOLDOWN 300


#include <allegro5/allegro.h>
#include "pistol.h"

typedef struct {
	int x;
	int y;
    int difficulty;
	int height;
	int width;
	int i;
	int j;
	pistol *gun;
} enemy;

typedef struct {
	int x;
	int y;
	int height;
	int width;
	int timer;
} motherShip;

typedef struct {
    float x;
    float y;
    int lifespan;  // Tempo de vida da explosão
} Explosion;

typedef struct {
	enemy **enemyMatrix;
	pistol *gun;
	int face;
	int enemyCount;
	Explosion *explosions;
	int explosionCount;
	int destroyedEnemyTimer;
} EnemyBoard;

EnemyBoard* create_enemyMatrix(unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, int mediumEnemyImageWidth, int mediumEnemyImageHeight);
motherShip* motherShip_create(int motherShipImageHeight, int motherShipImageWidth, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void add_explosion(EnemyBoard *enemyBoard, int lifespan, unsigned short i, unsigned short j);
void EnemyShot(EnemyBoard *enemyBoard, enemy *nearestEnemy);
int remove_enemy(enemy **enemyMatrix,int pos_x, int pos_y);

void free_enemyBoard(EnemyBoard *enemyBoard);


#endif

