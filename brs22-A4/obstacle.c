#include <stdlib.h>
#include <stdio.h>

#include "obstacle.h"

obstacle** obstacle_create(unsigned short x, unsigned short y, unsigned short height, unsigned short width, int max_x, int max_y){

    if ((x - width < 0) || (x + width > max_x) || (y - height/2 < 0) || (y + height > max_y)) return NULL;												
	/*if (face > 3) return NULL;*/																													

	obstacle **new_obstacle = (obstacle**) malloc(OBSTACLE_ROW * sizeof(obstacle*));																								
	if (!new_obstacle) return NULL;

    for (int i = 0; i < OBSTACLE_ROW; i++) {
        new_obstacle[i] = (obstacle*)malloc(OBSTACLE_COLUMN * sizeof(obstacle));
        if (!new_obstacle[i]) {
            printf("Falha de alocação de memória\n");
            return NULL;
        }
    }

    for (int i = 0; i < OBSTACLE_ROW; i++) {
        for (int j = 0; j < OBSTACLE_COLUMN; j++) {
            new_obstacle[i][j].width = width;
            new_obstacle[i][j].height = height;	
            new_obstacle[i][j].x = x + j * 20;
            new_obstacle[i][j].y = y + i * 20;
            new_obstacle[i][j].lifeCount = OBSTACLE_LIVES;
        }
    }

	return new_obstacle;			
}																												

void obstacle_collision(pistol *shotsList, obstacle **obstacleMatrix, int SquareCornerWidth, int SquareCornerHeight)
{
	bullet *previous;
  
	for (int i = 0; i <  OBSTACLE_ROW; i++) {
		for (int j = 0; j < OBSTACLE_COLUMN; j++) {
            if(i == 2 && (j == 1 || j == 2)) //nao precisa calcular a colisao nessas posicoes
				continue;

			previous = NULL;

			for (bullet *index = shotsList->shots; index != NULL;) {
				if ((((index->y-BULLET_H/2 >= obstacleMatrix[i][j].y-obstacleMatrix[i][j].height/2) && (obstacleMatrix[i][j].y+obstacleMatrix[i][j].height/2 >= index->y- BULLET_H/2)) || 				
                    ((obstacleMatrix[i][j].y-obstacleMatrix[i][j].height/2 >= index->y- BULLET_H/2) && (index->y+ BULLET_H/2 >= obstacleMatrix[i][j].y-obstacleMatrix[i][j].height/2))) && 	
                    (((index->x-BULLET_W/2 >= obstacleMatrix[i][j].x-obstacleMatrix[i][j].width/2) && (obstacleMatrix[i][j].x+obstacleMatrix[i][j].width/2 >= index->x-BULLET_W/2)) || 				
                    ((obstacleMatrix[i][j].x-obstacleMatrix[i][j].width/2 >= index->x-BULLET_W/2) && (index->x+BULLET_H/2 >= obstacleMatrix[i][j].x-obstacleMatrix[i][j].width/2)))){
						
                        if (obstacleMatrix[i][j].lifeCount){
                            if(index->type == 3 || index->type == 2)
                                obstacleMatrix[i][j].lifeCount -= 2;
                            else if(index->type == 1)
                                obstacleMatrix[i][j].lifeCount--;
                        }

						if (previous) {
							previous->next = index->next;
							bullet_destroy(index);
							index = (bullet *)previous->next;
						} else {
							shotsList->shots = (bullet *)index->next;
							bullet_destroy(index);
							index = shotsList->shots;
						}
					} else {
						previous = index;
						index = (bullet *)index->next;
					}
            }   
        
    }
}
}

    
void draw_obstacle(ALLEGRO_BITMAP* SquareCorner, ship *ship, EnemyBoard *enemyBoard,obstacle **obstacleMatrixUm,obstacle **obstacleMatrixDois,obstacle **obstacleMatrixTres,obstacle **obstacleMatrixQuatro,
 ALLEGRO_BITMAP* DestroyedObstacleOne, ALLEGRO_BITMAP* DestroyedObstacleTwo, ALLEGRO_BITMAP* DestroyedObstacleTree, ALLEGRO_BITMAP* DestroyedObstacleFour, ALLEGRO_BITMAP* DestroyedObstacleFive, 
 ALLEGRO_BITMAP* DestroyedObstacleSix, ALLEGRO_BITMAP* DestroyedObstacleSeven, ALLEGRO_BITMAP* DestroyedObstacleEight,ALLEGRO_BITMAP* DestroyedObstacleNine,ALLEGRO_BITMAP* DestroyedCornerOne, 
 ALLEGRO_BITMAP* DestroyedCornerTwo, ALLEGRO_BITMAP* DestroyedCornerTree) 
{

	int SquareCornerWidth = al_get_bitmap_width(SquareCorner);											 
	int SquareCornerHeight = al_get_bitmap_height(SquareCorner);
	int DestroyedObstacleWidth = al_get_bitmap_width(DestroyedObstacleOne);
	int DestroyedObstacleHeight =  al_get_bitmap_height(DestroyedObstacleOne);
	int DestroyedCornerWidth = al_get_bitmap_width(DestroyedCornerOne);
	int DestroyedCornerHeight = al_get_bitmap_height(DestroyedCornerOne);


    for (int i = 0; i < OBSTACLE_ROW; i++) {
    for (int j = 0; j < OBSTACLE_COLUMN; j++) {
        if(i == 2){
            if(j != 1 && j != 2){
                if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixUm[i][j].x-obstacleMatrixUm[i][j].width/2, obstacleMatrixUm[i][j].y-obstacleMatrixUm[i][j].width/2,obstacleMatrixUm[i][j].x+obstacleMatrixUm[i][j].width/2, obstacleMatrixUm[i][j].y+obstacleMatrixUm[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-1 && j == 0) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-1 && j == OBSTACLE_COLUMN-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-2 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-2 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-3 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-3 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-4 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-4 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-5 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-5 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-6 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-6 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-7 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-7 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-8 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-8 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-9 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else {
                    obstacleMatrixUm[i][j].x = -100;
                    obstacleMatrixUm[i][j].y = -100;
                }
            }

        } else if(i == 0 && j == 0){
            if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixUm[i][j].x - SquareCornerWidth /2 - 5, obstacleMatrixUm[i][j].y - SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight, 0);
            else if(obstacleMatrixUm[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixUm[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixUm[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight, 0);
            else {
                obstacleMatrixUm[i][j].x = -100;
                obstacleMatrixUm[i][j].y = -100;
                
            }
            }else if(i == 0 && j == OBSTACLE_COLUMN-1){
                if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixUm[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixUm[i][j].y -  SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixUm[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else {
                    obstacleMatrixUm[i][j].x = -100;
                    obstacleMatrixUm[i][j].y = -100;
                    
                }
            }else if(i == 1 && j == 1){
                if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixUm[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixUm[i][j].y + SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixUm[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else {
                    obstacleMatrixUm[i][j].x = -100;
                    obstacleMatrixUm[i][j].y = -100;
                    
                }

            }else if(i == 1 && j == 2){
                if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixUm[i][j].x - SquareCornerWidth / 2 - 5, obstacleMatrixUm[i][j].y +  SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixUm[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixUm[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixUm[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixUm[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixUm[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight, 0);
                else {
                    obstacleMatrixUm[i][j].x = -100;
                    obstacleMatrixUm[i][j].y = -100;
                    
                }
            }
                else{	
                if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixUm[i][j].x-obstacleMatrixUm[i][j].width/2, obstacleMatrixUm[i][j].y-obstacleMatrixUm[i][j].width/2,obstacleMatrixUm[i][j].x+obstacleMatrixUm[i][j].width/2, obstacleMatrixUm[i][j].y+obstacleMatrixUm[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixUm[i][j].lifeCount == OBSTACLE_LIVES-9)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixUm[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixUm[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else {
                    obstacleMatrixUm[i][j].x = -100;
                    obstacleMatrixUm[i][j].y = -100;
                }
            }
        }
    }

    for (int i = 0; i < OBSTACLE_ROW; i++) {
    for (int j = 0; j < OBSTACLE_COLUMN; j++) {
        if(i == 2){
            if(j != 1 && j != 2){
                if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixDois[i][j].x-obstacleMatrixDois[i][j].width/2, obstacleMatrixDois[i][j].y-obstacleMatrixDois[i][j].width/2,obstacleMatrixDois[i][j].x+obstacleMatrixDois[i][j].width/2, obstacleMatrixDois[i][j].y+obstacleMatrixDois[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-1 && j == 0) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-1 && j == OBSTACLE_COLUMN-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-2 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-2 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-3 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-3 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-4 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-4 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-5 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-5 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-6 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-6 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-7 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-7 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-8 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-8 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-9 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else{
                    obstacleMatrixDois[i][j].x = -100;
                    obstacleMatrixDois[i][j].y = -100;
                }
            }
        } else if(i == 0 && j == 0){
            if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixDois[i][j].x - SquareCornerWidth /2 - 5, obstacleMatrixDois[i][j].y - SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight, 0);
            else if(obstacleMatrixDois[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixDois[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixDois[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight, 0);
            else {
                obstacleMatrixDois[i][j].x = -100;
                obstacleMatrixDois[i][j].y = -100; 
            }

            }else if(i == 0 && j == OBSTACLE_COLUMN-1){
                if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixDois[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixDois[i][j].y -  SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixDois[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else {
                    obstacleMatrixDois[i][j].x = -100;
                    obstacleMatrixDois[i][j].y = -100;
                    
                }
            }else if(i == 1 && j == 1){
                if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixDois[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixDois[i][j].y + SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixDois[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else {
                    obstacleMatrixDois[i][j].x = -100;
                    obstacleMatrixDois[i][j].y = -100;
                    
                }

            }else if(i == 1 && j == 2){
                if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixDois[i][j].x - SquareCornerWidth / 2 - 5, obstacleMatrixDois[i][j].y +  SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixDois[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixDois[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixDois[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixDois[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixDois[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight, 0);
                else{
                    obstacleMatrixDois[i][j].x = -100;
                    obstacleMatrixDois[i][j].y = -100;
                    
                }
            }
                else{	
                if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixDois[i][j].x-obstacleMatrixDois[i][j].width/2, obstacleMatrixDois[i][j].y-obstacleMatrixDois[i][j].width/2,obstacleMatrixDois[i][j].x+obstacleMatrixDois[i][j].width/2, obstacleMatrixDois[i][j].y+obstacleMatrixDois[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixDois[i][j].lifeCount == OBSTACLE_LIVES-9)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixDois[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixDois[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else{
                    obstacleMatrixDois[i][j].x = -100;
                    obstacleMatrixDois[i][j].y = -100;
                }
            }
        }
    }

    for (int i = 0; i < OBSTACLE_ROW; i++) {
    for (int j = 0; j < OBSTACLE_COLUMN; j++) {
        if(i == 2){
            if(j != 1 && j != 2){
                if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixTres[i][j].x-obstacleMatrixTres[i][j].width/2, obstacleMatrixTres[i][j].y-obstacleMatrixTres[i][j].width/2,obstacleMatrixTres[i][j].x+obstacleMatrixTres[i][j].width/2, obstacleMatrixTres[i][j].y+obstacleMatrixTres[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-1 && j == 0) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-1 && j == OBSTACLE_COLUMN-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-2 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-2 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-3 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-3 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-4 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-4 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-5 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-5 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-6 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-6 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-7 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-7 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-8 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-8 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-9 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                
                else{
                    obstacleMatrixTres[i][j].x = -100;
                    obstacleMatrixTres[i][j].y = -100;
                }
            }
        } else if(i == 0 && j == 0){
            if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixTres[i][j].x - SquareCornerWidth /2 - 5, obstacleMatrixTres[i][j].y - SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight, 0);
            else if(obstacleMatrixTres[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixTres[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixTres[i][j].lifeCount >= OBSTACLE_LIVES - 9  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight, 0);
            else {
                obstacleMatrixTres[i][j].x = -100;
                obstacleMatrixTres[i][j].y = -100;
                
            }
            }else if(i == 0 && j == OBSTACLE_COLUMN-1){
                if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixTres[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixTres[i][j].y -  SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount > OBSTACLE_LIVES - 4  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount >=  OBSTACLE_LIVES - 5  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount >= OBSTACLE_LIVES - 9  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixTres[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else{
                    obstacleMatrixTres[i][j].x = -100;
                    obstacleMatrixTres[i][j].y = -100;
                    
                }
            }else if(i == 1 && j == 1){
                if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixTres[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixTres[i][j].y + SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount > OBSTACLE_LIVES - 4  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount >=  OBSTACLE_LIVES - 5  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixTres[i][j].lifeCount >= OBSTACLE_LIVES - 9  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else{
                    obstacleMatrixTres[i][j].x = -100;
                    obstacleMatrixTres[i][j].y = -100;
                    
                }

            }else if(i == 1 && j == 2){
                if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixTres[i][j].x - SquareCornerWidth / 2 - 5, obstacleMatrixTres[i][j].y +  SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount > OBSTACLE_LIVES - 4  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixTres[i][j].lifeCount >=  OBSTACLE_LIVES - 5  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixTres[i][j].lifeCount >= OBSTACLE_LIVES - 9  && obstacleMatrixTres[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixTres[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixTres[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight, 0);
                else {
                    obstacleMatrixTres[i][j].x = -100;
                    obstacleMatrixTres[i][j].y = -100;
                    
                }
            }
                else{	
                if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixTres[i][j].x-obstacleMatrixTres[i][j].width/2, obstacleMatrixTres[i][j].y-obstacleMatrixTres[i][j].width/2,obstacleMatrixTres[i][j].x+obstacleMatrixTres[i][j].width/2, obstacleMatrixTres[i][j].y+obstacleMatrixTres[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixTres[i][j].lifeCount == OBSTACLE_LIVES-9)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixTres[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixTres[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else {
                    obstacleMatrixTres[i][j].x = -100;
                    obstacleMatrixTres[i][j].y = -100;
                }
            }
        }
    }


	
    for (int i = 0; i < OBSTACLE_ROW; i++) {
    for (int j = 0; j < OBSTACLE_COLUMN; j++) {
        if(i == 2){
            if(j != 1 && j != 2){
                if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixQuatro[i][j].x-obstacleMatrixQuatro[i][j].width/2, obstacleMatrixQuatro[i][j].y-obstacleMatrixQuatro[i][j].width/2,obstacleMatrixQuatro[i][j].x+obstacleMatrixQuatro[i][j].width/2, obstacleMatrixQuatro[i][j].y+obstacleMatrixQuatro[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-1 && j == 0) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-1 && j == OBSTACLE_COLUMN-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-2 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-2 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-3 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-3 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-4 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-4 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-5 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-5 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-6 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-6 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-7 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-7 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-8 && j == 0)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-8 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-9 && j == OBSTACLE_COLUMN-1)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else{
                    obstacleMatrixQuatro[i][j].x = -100;
                    obstacleMatrixQuatro[i][j].y = -100;
                }
            }
        } else if(i == 0 && j == 0){
            if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixQuatro[i][j].x - SquareCornerWidth /2 - 5, obstacleMatrixQuatro[i][j].y - SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight, 0);
            else if(obstacleMatrixQuatro[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixQuatro[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  0);
            else if(obstacleMatrixQuatro[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight, 0);
            else{
                obstacleMatrixQuatro[i][j].x = -100;
                obstacleMatrixQuatro[i][j].y = -100;
                
            }
            }else if(i == 0 && j == OBSTACLE_COLUMN-1){
                if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixQuatro[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixQuatro[i][j].y -  SquareCornerHeight/2 + 11, SquareCornerWidth, SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixQuatro[i][j].y -  DestroyedCornerHeight/2, DestroyedCornerWidth, DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else{
                    obstacleMatrixQuatro[i][j].x = -100;
                    obstacleMatrixQuatro[i][j].y = -100;
                    
                }
            }else if(i == 1 && j == 1){
                if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixQuatro[i][j].x - SquareCornerWidth / 2 + 5, obstacleMatrixQuatro[i][j].y + SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else if(obstacleMatrixQuatro[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2 - 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  ALLEGRO_FLIP_HORIZONTAL);
                else {
                    obstacleMatrixQuatro[i][j].x = -100;
                    obstacleMatrixQuatro[i][j].y = -100;
                    
                }

            }else if(i == 1 && j == 2){
                if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(SquareCorner, 0, 0, al_get_bitmap_width(SquareCorner), al_get_bitmap_height(SquareCorner), obstacleMatrixQuatro[i][j].x - SquareCornerWidth / 2 - 5, obstacleMatrixQuatro[i][j].y +  SquareCornerHeight/2 - 11, SquareCornerWidth, -SquareCornerHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount > OBSTACLE_LIVES - 4 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerOne, 0, 0, al_get_bitmap_width(DestroyedCornerOne), al_get_bitmap_height(DestroyedCornerOne), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixQuatro[i][j].lifeCount >=  OBSTACLE_LIVES - 5 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTwo, 0, 0, al_get_bitmap_width(DestroyedCornerTwo), al_get_bitmap_height(DestroyedCornerTwo), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight,  0);
                else if(obstacleMatrixQuatro[i][j].lifeCount >= OBSTACLE_LIVES - 9 && obstacleMatrixQuatro[i][j].lifeCount < OBSTACLE_LIVES)
                    al_draw_scaled_bitmap(DestroyedCornerTree, 0, 0, al_get_bitmap_width(DestroyedCornerTree), al_get_bitmap_height(DestroyedCornerTree), obstacleMatrixQuatro[i][j].x - DestroyedCornerWidth / 2 + 2, obstacleMatrixQuatro[i][j].y +  DestroyedCornerHeight/2, DestroyedCornerWidth, -DestroyedCornerHeight, 0);
                else {
                    obstacleMatrixQuatro[i][j].x = -100;
                    obstacleMatrixQuatro[i][j].y = -100;
                    
                }
            }
                else{	
                if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES)
                    al_draw_filled_rectangle(obstacleMatrixQuatro[i][j].x-obstacleMatrixQuatro[i][j].width/2, obstacleMatrixQuatro[i][j].y-obstacleMatrixQuatro[i][j].width/2,obstacleMatrixQuatro[i][j].x+obstacleMatrixQuatro[i][j].width/2, obstacleMatrixQuatro[i][j].y+obstacleMatrixQuatro[i][j].width/2, al_map_rgb(51, 255, 0));
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-1) 
                    al_draw_scaled_bitmap(DestroyedObstacleOne, 0, 0, al_get_bitmap_width(DestroyedObstacleOne), al_get_bitmap_height(DestroyedObstacleOne), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-2)
                    al_draw_scaled_bitmap(DestroyedObstacleTwo, 0, 0, al_get_bitmap_width(DestroyedObstacleTwo), al_get_bitmap_height(DestroyedObstacleTwo), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-3)
                    al_draw_scaled_bitmap(DestroyedObstacleTree, 0, 0, al_get_bitmap_width(DestroyedObstacleTree), al_get_bitmap_height(DestroyedObstacleTree), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-4)
                    al_draw_scaled_bitmap(DestroyedObstacleFour, 0, 0, al_get_bitmap_width(DestroyedObstacleFour), al_get_bitmap_height(DestroyedObstacleFour), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-5)
                    al_draw_scaled_bitmap(DestroyedObstacleFive, 0, 0, al_get_bitmap_width(DestroyedObstacleFive), al_get_bitmap_height(DestroyedObstacleFive), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-6)
                    al_draw_scaled_bitmap(DestroyedObstacleSix, 0, 0, al_get_bitmap_width(DestroyedObstacleSix), al_get_bitmap_height(DestroyedObstacleSix), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-7)
                    al_draw_scaled_bitmap(DestroyedObstacleSeven, 0, 0, al_get_bitmap_width(DestroyedObstacleSeven), al_get_bitmap_height(DestroyedObstacleSeven), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-8)
                    al_draw_scaled_bitmap(DestroyedObstacleEight, 0, 0, al_get_bitmap_width(DestroyedObstacleEight), al_get_bitmap_height(DestroyedObstacleEight), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 - 5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else if(obstacleMatrixQuatro[i][j].lifeCount == OBSTACLE_LIVES-9)
                    al_draw_scaled_bitmap(DestroyedObstacleNine, 0, 0, al_get_bitmap_width(DestroyedObstacleNine), al_get_bitmap_height(DestroyedObstacleNine), obstacleMatrixQuatro[i][j].x - DestroyedObstacleWidth / 2 -5, obstacleMatrixQuatro[i][j].y -  DestroyedObstacleHeight /2 + 11, DestroyedObstacleWidth, DestroyedObstacleHeight, 0);
                else {
                    obstacleMatrixQuatro[i][j].x = -100;
                    obstacleMatrixQuatro[i][j].y = -100;
                }
            }
        }
    }

	
	obstacle_collision(ship->gun,obstacleMatrixUm,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(enemyBoard->gun,obstacleMatrixUm,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(ship->gun,obstacleMatrixDois,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(enemyBoard->gun,obstacleMatrixDois,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(ship->gun,obstacleMatrixTres,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(enemyBoard->gun,obstacleMatrixTres,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(ship->gun,obstacleMatrixQuatro,SquareCornerWidth, SquareCornerHeight);
	obstacle_collision(enemyBoard->gun,obstacleMatrixQuatro,SquareCornerWidth, SquareCornerHeight);

}

void obstacle_destroy(obstacle ***obstacleMatrix) {
    for (int i = 0; i < OBSTACLE_ROW; i++) {
        free((*obstacleMatrix)[i]);
    }
    free(*obstacleMatrix);
    *obstacleMatrix = NULL;
}
