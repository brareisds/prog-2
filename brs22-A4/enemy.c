#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"

EnemyBoard* create_enemyMatrix(unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, int mediumEnemyImageWidth, int mediumEnemyImageHeight) {
   
    if (max_x <= 0 || max_y <= 0) {
        printf("Dimensões inválidas\n");
        return NULL;
    }

    EnemyBoard *enemyBoard = (EnemyBoard*)malloc(sizeof(EnemyBoard));
    if (!enemyBoard) {
        printf("Falha de alocação de memória para EnemyBoard\n");
        return NULL;
    }

    enemyBoard->explosions = (Explosion*)malloc(MAX_EXPLOSIONS * sizeof(Explosion));
    if (!enemyBoard->explosions) {
        printf("Falha de alocação de memória para EnemyBoard\n");
        return NULL;
    }

    for (int i = 0; i < MAX_EXPLOSIONS; i ++) {
        enemyBoard->explosions[i].lifespan = 0;
        enemyBoard->explosionCount = 0;
    }
    
    enemyBoard->enemyCount = ENEMY_COLUMNS * ENEMY_ROWS;
    enemyBoard->face = 3;
    enemyBoard->gun = pistol_create();
    enemyBoard->destroyedEnemyTimer = 0;
    enemyBoard->enemyMatrix = (enemy**)malloc(ENEMY_ROWS * sizeof(enemy*));
   

    if (!enemyBoard->enemyMatrix) {
        printf("Falha de alocação de memória para enemyMatrix\n");
        free(enemyBoard);
        return NULL;
    }

    for (int i = 0; i < ENEMY_ROWS; i++) {
        enemyBoard->enemyMatrix[i] = (enemy*)malloc(ENEMY_COLUMNS * sizeof(enemy));
        if (!enemyBoard->enemyMatrix[i]) {
            printf("Falha de alocação de memória para enemyMatrix[%d]\n", i);
            return NULL;
        }
    }

    for (int i = 0; i < ENEMY_ROWS; i++) {
        for (int j = 0; j < ENEMY_COLUMNS; j++) {
            enemyBoard->enemyMatrix[i][j].x = x + j * (mediumEnemyImageWidth + ESPACAMENTO_ENTRE_ALIENS);
            enemyBoard->enemyMatrix[i][j].y = y + i * (mediumEnemyImageHeight + ESPACAMENTO_ENTRE_ALIENS);
            enemyBoard->enemyMatrix[i][j].height = mediumEnemyImageHeight;
            enemyBoard->enemyMatrix[i][j].width = mediumEnemyImageWidth;
            if (i == 0)
                enemyBoard->enemyMatrix[i][j].difficulty = 3;
            else if (i == 1 || i == 2)
                enemyBoard->enemyMatrix[i][j].difficulty = 2;
            else if (i == 3 || i == 4)
                enemyBoard->enemyMatrix[i][j].difficulty = 1;
        }
    }

    return enemyBoard;
}

motherShip* motherShip_create(int motherShipImageHeight, int motherShipImageWidth, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){
   
    if (max_x <= 0 || max_y <= 0) {
        printf("Dimensões inválidas\n");
        return NULL;
    }

    motherShip *MotherShip = (motherShip*)malloc(sizeof(motherShip));
    if (!MotherShip) {
        printf("Falha de alocação de memória para mother ship\n");
        return NULL;
    }

    MotherShip->timer = MOTHER_SHIP_COOLDOWN;
    MotherShip->x = x;
    MotherShip->y = y;
    MotherShip->height = motherShipImageHeight;
    MotherShip->width =  motherShipImageWidth;

    return MotherShip;
}

void EnemyShot(EnemyBoard *enemyBoard, enemy *nearestEnemy){																														
	bullet *shot;
	if (enemyBoard->face == 3) shot = enemy_pistol_shot(nearestEnemy->x + nearestEnemy->width/2, nearestEnemy->y, enemyBoard->face, nearestEnemy->difficulty, enemyBoard->gun);									
	if (shot) enemyBoard->gun->shots = shot;
}

void add_explosion(EnemyBoard *enemyBoard, int lifespan, unsigned short i, unsigned short j)
{
    unsigned short explosionIndex = enemyBoard->explosionCount; // Armazena a posição atual
    if(explosionIndex > MAX_EXPLOSIONS)
        return;

    enemyBoard->explosions[explosionIndex].x = enemyBoard->enemyMatrix[i][j].x;
    enemyBoard->explosions[explosionIndex].y = enemyBoard->enemyMatrix[i][j].y;
    enemyBoard->explosions[explosionIndex].lifespan = lifespan;  // Defina um tempo de vida para a explosão

    // Incrementa o contador de explosões apenas depois de adicionar a explosão
    enemyBoard->explosionCount++;
}

/* Remove o inimigo da tela quando ele eh atingido*/
int remove_enemy(enemy **enemyMatrix, int row, int col) {

    enemyMatrix[row][col].x = -100;
    enemyMatrix[row][col].y = -100;
    
}

void free_enemyBoard(EnemyBoard *enemyBoard) {
    // Liberar explosões
    free(enemyBoard->explosions);

    // Liberar matriz de inimigos
    for (int i = 0; i < ENEMY_ROWS; i++) {
        free(enemyBoard->enemyMatrix[i]);
    }
    free(enemyBoard->enemyMatrix);

    free(enemyBoard->gun);
    free(enemyBoard);
}



