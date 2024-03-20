#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdio.h>

#include "ship.h"
#include "enemy.h"
#include "obstacle.h"

#define SCREEN_W 1280
#define SCREEN_H 720
#define FPS 30.0
#define ENEMY_FRAME 22.5

void draw_background(ship *ship,  ALLEGRO_BITMAP* ShipImage, ALLEGRO_FONT* font) 
{
	char scoreText[20];
    al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(font, al_map_rgb(255, 255, 255), 270, 25, 0, "SCORE");
	// Ajuste o tamanho conforme necessário
    sprintf(scoreText, "%d", ship->score);

    // Desenha a pontuação ao lado do texto "SCORE"
    al_draw_text(font, al_map_rgb(51, 255, 0), 390, 25, 0, scoreText);


    int lineStartX = 40;  
    int lineEndX = SCREEN_W - 40;  
    int lineY = SCREEN_H - 18; 

	int startX = SCREEN_W - 490;  																																	

	/* Espaçamento entre as naves */
	int shipSpacing = 75; 
	int ShipImageWidth = al_get_bitmap_width(ShipImage) * 2;											 
	int ShipImageHeight = al_get_bitmap_height(ShipImage) * 2;

	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W - 600, 25, 0, "LIVES");

	for(int i = 0; i < ship->lifeCount; i++){ 																											
		int x = startX + i * shipSpacing;  																												
		al_draw_scaled_bitmap(ShipImage, 0, 0, al_get_bitmap_width(ShipImage), al_get_bitmap_height(ShipImage), x, 15, ShipImageWidth, ShipImageHeight, 0); 		
	}
		

    al_draw_line(lineStartX, lineY, lineEndX, lineY, al_map_rgb(51, 255, 0), 2);
}


int check_enemy_kill(ship *ship, EnemyBoard *enemyBoard, ALLEGRO_BITMAP* ExplosionImage) {

    bullet *previous;
    
	for (int i = 0; i < ENEMY_ROWS; i++) {
		for (int j = 0; j < ENEMY_COLUMNS; j++) {
			previous = NULL;
			for (bullet *index = ship->gun->shots; index != NULL;) 
			{
				if ((((index->y >= enemyBoard->enemyMatrix[i][j].y) && (enemyBoard->enemyMatrix[i][j].y + enemyBoard->enemyMatrix[i][j].height >= index->y))) &&
					(((enemyBoard->enemyMatrix[i][j].x >= index->x) && (index->x + BULLET_W >= enemyBoard->enemyMatrix[i][j].x )) ||
						((index->x >= enemyBoard->enemyMatrix[i][j].x) && (enemyBoard->enemyMatrix[i][j].x + enemyBoard->enemyMatrix[i][j].width >= index->x))))
				{
					add_explosion(enemyBoard,4, i, j);
					remove_enemy(enemyBoard->enemyMatrix, i, j);
	
					/* Remove o tiro */
					if (previous) {
						previous->next = index->next;
						bullet_destroy(index);
						index = (bullet *)previous->next;
					} else {
						ship->gun->shots = (bullet *)index->next;
						bullet_destroy(index);
						index = ship->gun->shots;
					} 
					if(enemyBoard->enemyCount) enemyBoard->enemyCount--;
					if(enemyBoard->enemyMatrix[i][j].difficulty == 3)
						ship->score += 40;
					else if(enemyBoard->enemyMatrix[i][j].difficulty == 2)
						ship->score += 20;
					else if(enemyBoard->enemyMatrix[i][j].difficulty == 1)
						ship->score += 10;

				}
				else {
					previous = index;
					index = (bullet *)index->next;
				}
					
			}
	
		}

    }

	/* Verifica se todos os inimigos foram mortos */
	if(!enemyBoard->enemyCount){
		return 1;
	}
	else	
		return 0; 
}

void check_shipMother_kill(ship *ship, motherShip *motherShip) {

    bullet *previous;

	previous = NULL;
	for (bullet *index = ship->gun->shots; index != NULL;) 
	{ 
		if ((((index->y >= motherShip->y) && (motherShip->y + motherShip->height >= index->y))) &&
        (((motherShip->x + motherShip->width >= index->x) && (index->x >= motherShip->x)) ||
        ((index->x >= motherShip->x) && (index->x <= motherShip->x + motherShip->width))))
    	{
			/* Remove o tiro */
			if (previous) {
				previous->next = index->next;
				bullet_destroy(index);
				index = (bullet *)previous->next;
			} else {
				ship->gun->shots = (bullet *)index->next;
				bullet_destroy(index);
				index = ship->gun->shots;
			} 
			
			motherShip->x = SCREEN_W + 20;
			ship->score += 300;
		}
		else {
			previous = index;
			index = (bullet *)index->next;
		}
	}
	
}


int check_ship_kill(ship *ship, EnemyBoard *enemyBoard) 
{
	bullet *previous = NULL;
	for (bullet *index = enemyBoard->gun->shots; index != NULL;) {
		if ((((ship->y >= index->y - BULLET_H) &&
			(index->y + BULLET_H >= ship->y)) ||
			((index->y - BULLET_H >= ship->y) &&
			(ship->y >= index->y - BULLET_H))) &&
		(((ship->x - (ship->width/2)+20 >= index->x - BULLET_W) &&
			(index->x + BULLET_W >= ship->x - (ship->width/2)+20)) ||
			((index->x - BULLET_W >= ship->x - (ship->width/2)+20) &&
			(ship->x + (ship->width/2)+20 >= index->x - BULLET_W))))
		{
			if (previous) {
				previous->next = index->next;
				bullet_destroy(index);
				index = (bullet *)previous->next;
			} else {
				enemyBoard->gun->shots = (bullet *)index->next;
				bullet_destroy(index);
				index = enemyBoard->gun->shots;
			}

			if(ship->lifeCount) ship->lifeCount--;
			ship->destroyedShipTimer = 60;

        } else {
            previous = index;
            index = (bullet *)index->next;
        }
	}

	/* Remove todos os inimigos da tela */
	if(!ship->lifeCount){ 	
		for(int i = 0; i < ENEMY_ROWS; i++){
			for (int j = 0; j < ENEMY_COLUMNS; j++)
			{
				enemyBoard->enemyMatrix[i][j].x = -100;
				enemyBoard->enemyMatrix[i][j].y = -100;
			}
		}																		
		return 1;
	}
	else
		return 0;	
}


void update_bullets(ship *ship, EnemyBoard *enemyBoard){
    bullet *previousShip = NULL; 
    for (bullet *ShipIndex = ship->gun->shots; ShipIndex != NULL;){
        if (ShipIndex->trajectory == 2) ShipIndex->y -= BULLET_MOVE;
        if ((ShipIndex->y > SCREEN_H || ShipIndex->y < 0)){
            if (previousShip){
                previousShip->next = ShipIndex->next;
                bullet_destroy(ShipIndex);
                ShipIndex = (bullet *) previousShip->next;
            } else {
            ship->gun->shots = (bullet *) ShipIndex->next;
                bullet_destroy(ShipIndex);
                ShipIndex = ship->gun->shots;
            }
        } else {
            previousShip = ShipIndex;
            ShipIndex = (bullet *) ShipIndex->next;
        }
    }
	
	bullet *previousEnemy = NULL; 
    for (bullet *EnemyIndex = enemyBoard->gun->shots; EnemyIndex != NULL;){
        if (EnemyIndex->trajectory == 3) EnemyIndex->y += BULLET_ENEMY_MOVE;
        if ((EnemyIndex->y >= SCREEN_H - 28 || EnemyIndex->y < 0)){
            if (previousEnemy){
                previousEnemy->next = EnemyIndex->next;
                bullet_destroy(EnemyIndex);
                EnemyIndex = (bullet *) previousEnemy->next;
            } else {
            enemyBoard->gun->shots = (bullet *) EnemyIndex->next;
                bullet_destroy(EnemyIndex);
                EnemyIndex = enemyBoard->gun->shots;
            }
        } else {
            previousEnemy = EnemyIndex;
            EnemyIndex = (bullet *) EnemyIndex->next;
        }
    }

}

// Função que calcula a distância entre dois pontos
double calculateDistance(ship *ship, enemy nearestEnemy) {
    int dx = ship->x - nearestEnemy.x;
    int dy = ship->y - nearestEnemy.y;
    
    return sqrt(dx * dx + dy * dy);
}

// Função que encontra o inimigo mais próximo de uma nave
enemy findNearestEnemy(ship *ship, enemy **enemyMatrix, int RowLimit, int ColumnLimit) {
    if (!enemyMatrix) {
        printf("matriz de inimigos vazia\n");
    }

    enemy nearestEnemy = enemyMatrix[0][0];

    double minDistance = calculateDistance(ship, enemyMatrix[0][0]);

	for (int i = 0; i < RowLimit; i++) {
        for (int j = 0; j < ColumnLimit; j++) {
			double distance = calculateDistance(ship, enemyMatrix[i][j]);
			if (distance < minDistance) {
				minDistance = distance;
				nearestEnemy = enemyMatrix[i][j];
				nearestEnemy.i = i;
				nearestEnemy.j = j;
        	}
		}
	}
	
	return nearestEnemy;
}

int ALEAT(int min, int max)
{
    return (rand() % (max -  min + 1) + min);
}

void update_position(ship* ship, EnemyBoard *enemyBoard, motherShip *motherShip){																																				

	int ShipColumnContainsShot;
	int EnemyColumnContainsShot;
	enemy nearestEnemy; 

	if (ship->control->left){																																											
		ship_move(ship, 1, 0, SCREEN_W); 																																									
	}
	if (ship->control->right){																																											
		ship_move(ship, 1, 1, SCREEN_W); 																																												
	}	
	if (ship->control->fire){	
		ShipColumnContainsShot = 0;
		bullet *previous = NULL;

		/* Verifica se ja existe um tiro na coluna */ 
    	for (bullet *index = ship->gun->shots; index != NULL & !ShipColumnContainsShot;){
	
			if(index->x == ship->x + ship->width/2){
				ShipColumnContainsShot = 1;
			}
			previous = index;
            index = (bullet *) index->next;
        }
	
		if(!ShipColumnContainsShot && !ship->gun->timer){																																																																																					
			ship_shot(ship);																																											
			ship->gun->timer = PISTOL_COOLDOWN;																																							
		}
	}	

	int RowLimit = ENEMY_ROWS; 
	int ColumnLimit = ENEMY_COLUMNS;

	/* Verifica se eh possivel o inimigo atirar e escolhe o inimigo mais proximo e um inimigo aleatorio mais proximo */
	if(!enemyBoard->gun->timer){

		nearestEnemy = findNearestEnemy(ship, enemyBoard->enemyMatrix, RowLimit, ColumnLimit);

		bullet *previousEnemy = NULL; 
		EnemyColumnContainsShot = 0;
		for (bullet *index = enemyBoard->gun->shots; index != NULL && !EnemyColumnContainsShot;){
			if(index->x == nearestEnemy.x + nearestEnemy.width/2){
				EnemyColumnContainsShot = 1;
			}
			previousEnemy = index;
            index = (bullet *) index->next;
        }
			if(nearestEnemy.difficulty < 3){
			if(!EnemyColumnContainsShot){
				if(nearestEnemy.difficulty == 1)
				{	
					if(nearestEnemy.i < ENEMY_ROWS-1)
					{
						if(!&enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j] || enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j].x < 0 
						|| enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j].y < 0){
			
							EnemyShot(enemyBoard,&nearestEnemy);
						}
					} else{
		
						EnemyShot(enemyBoard,&nearestEnemy);
					}
				} else if(nearestEnemy.difficulty == 2){
					EnemyShot(enemyBoard,&nearestEnemy);
				
				} 
			} 
		} else if (nearestEnemy.difficulty == 3){
			EnemyShot(enemyBoard,&nearestEnemy);
		}

		/* Define um raio para calcular o novo inimigo mais proximo dentro dele */
		RowLimit = ALEAT(0,ENEMY_ROWS-1);
		ColumnLimit = ALEAT(0,ENEMY_COLUMNS-1);
		nearestEnemy = findNearestEnemy(ship, enemyBoard->enemyMatrix, RowLimit, ColumnLimit);
		
		previousEnemy = NULL; 
		EnemyColumnContainsShot = 0;
		for (bullet *index = enemyBoard->gun->shots; index != NULL && !EnemyColumnContainsShot;){
	
			if(index->x == nearestEnemy.x + nearestEnemy.width/2){
				EnemyColumnContainsShot = 1;
			}
			previousEnemy = index;
            index = (bullet *) index->next;
        }

		if(nearestEnemy.difficulty < 3){
			if(!EnemyColumnContainsShot){
				if(nearestEnemy.difficulty == 1)
				{	
					if(nearestEnemy.i < ENEMY_ROWS-1)
					{
						if(!&enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j] || enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j].x < 0 
						|| enemyBoard->enemyMatrix[nearestEnemy.i+1][nearestEnemy.j].y < 0){
			
							EnemyShot(enemyBoard,&nearestEnemy);
						}
					} else{
		
						EnemyShot(enemyBoard,&nearestEnemy);
					}

					
				} else if(nearestEnemy.difficulty == 2){
					EnemyShot(enemyBoard,&nearestEnemy);
				
				} 
			} 
		} else if (nearestEnemy.difficulty == 3){
			EnemyShot(enemyBoard,&nearestEnemy);
		}

		enemyBoard->gun->timer = ENEMY_PISTOL_COOLDOWN;
	}	

	if(!motherShip->timer){
		motherShip->timer = MOTHER_SHIP_COOLDOWN;
	
		if(motherShip->x > SCREEN_W) motherShip->x = -10;
	} else 
		motherShip->x += MOTHER_SHIP_MOVE;	

	update_bullets(ship,enemyBoard);

}



int main(){
	al_init();																		
	al_install_keyboard();															
    al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
  
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);								
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT *font = al_load_ttf_font("fonts/space_invaders.ttf", 24,0);
	ALLEGRO_FONT *small_font = al_load_ttf_font("fonts/space_invaders.ttf", 18,0);
	ALLEGRO_FONT *font_title = al_load_ttf_font("fonts/space_invaders.ttf", 36,0);
									
	ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_W, SCREEN_H);	
    ALLEGRO_BITMAP* ShipImage = al_load_bitmap("images/nave.bmp");
	ALLEGRO_BITMAP* weakEnemyImageUm = al_load_bitmap("images/weakEnemyUm.bmp");
	ALLEGRO_BITMAP* weakEnemyImageDois = al_load_bitmap("images/weakEnemyDois.bmp");
	ALLEGRO_BITMAP* mediumEnemyImageUm = al_load_bitmap("images/mediumEnemyUm.bmp");
	ALLEGRO_BITMAP* mediumEnemyImageDois = al_load_bitmap("images/mediumEnemyDois.bmp");
	ALLEGRO_BITMAP* StrongEnemyImageUm = al_load_bitmap("images/strongEnemyUm.bmp");
	ALLEGRO_BITMAP* StrongEnemyImageDois = al_load_bitmap("images/StrongEnemyDois.bmp");
	ALLEGRO_BITMAP* WeakShot = al_load_bitmap("images/weakShot.bmp");
	ALLEGRO_BITMAP* MediumShot = al_load_bitmap("images/MediumShot.bmp");
	ALLEGRO_BITMAP* StrongShot = al_load_bitmap("images/StrongShot.bmp");
	ALLEGRO_BITMAP* SquareCorner = al_load_bitmap("images/SquareCorner.bmp");
	ALLEGRO_BITMAP* ExplosionImage = al_load_bitmap("images/explosion.bmp");
	ALLEGRO_BITMAP* ShipDestroyed = al_load_bitmap("images/ShipDestroyed.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleOne = al_load_bitmap("images/DestroyedObtacleOne.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleTwo = al_load_bitmap("images/DestroyedObtacleTwo.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleTree = al_load_bitmap("images/DestroyedObtacleTree.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleFour = al_load_bitmap("images/DestroyedObtacleFour.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleFive = al_load_bitmap("images/DestroyedObtacleFIve.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleSix = al_load_bitmap("images/DestroyedObtacleSix.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleSeven = al_load_bitmap("images/DestroyedObtacleSeven.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleEight = al_load_bitmap("images/DestroyedObtacleEight.bmp");
	ALLEGRO_BITMAP* DestroyedObstacleNine = al_load_bitmap("images/DestroyedObtacleNine.bmp");
	ALLEGRO_BITMAP* DestroyedCornerOne = al_load_bitmap("images/CornerDestroyedOne.bmp");
	ALLEGRO_BITMAP* DestroyedCornerTwo = al_load_bitmap("images/CornerDestroyedTwo.bmp");
	ALLEGRO_BITMAP* DestroyedCornerTree = al_load_bitmap("images/CornerDestroyedTree.bmp");
	ALLEGRO_BITMAP* SpaceInvaders = al_load_bitmap("images/SpaceInvaders.bmp");
	ALLEGRO_BITMAP* arrowImage = al_load_bitmap("images/arrow.bmp");
	ALLEGRO_BITMAP* spaceImage = al_load_bitmap("images/space.bmp");
	ALLEGRO_BITMAP* motherShipImage = al_load_bitmap("images/motherShip.bmp");

    if (!ShipImage) {
        fprintf(stderr, "Falha ao carregar a imagem da nave: nave.bmp\n");
        return -1; 
    }
	if (!mediumEnemyImageUm) {
        fprintf(stderr, "Falha ao carregar mediumEnemyImageUm\n");
        return -1; 
    }
	if (!mediumEnemyImageDois) {
        fprintf(stderr, "Falha ao carregar mediumEnemyImageDois\n");
        return -1; 
    }
						
	al_register_event_source(queue, al_get_keyboard_event_source());				
	al_register_event_source(queue, al_get_display_event_source(disp));				
	al_register_event_source(queue, al_get_timer_event_source(timer));	

	/* Ajusta a proporção da imagem */
	int ShipImageWidth = al_get_bitmap_width(ShipImage) * 2;											 
	int ShipImageHeight = al_get_bitmap_height(ShipImage) * 2;
	int weakEnemyImageUmWidth = al_get_bitmap_width(weakEnemyImageUm) * 2;
	int weakEnemyImageUmHeight = al_get_bitmap_height(weakEnemyImageUm) * 2;
	int weakEnemyImageDoisWidth = al_get_bitmap_width(weakEnemyImageDois) * 2;
	int weakEnemyImageDoisHeight = al_get_bitmap_height(weakEnemyImageDois) * 2;
	int mediumEnemyImageUmWidth = al_get_bitmap_width(mediumEnemyImageUm) * 2;											 
	int mediumEnemyImageUmHeight = al_get_bitmap_height(mediumEnemyImageUm) * 2;
	int mediumEnemyImageDoisWidth = al_get_bitmap_width(mediumEnemyImageDois) * 2;											 
	int mediumEnemyImageDoisHeight = al_get_bitmap_height(mediumEnemyImageDois) * 2;
	int StrongEnemyImageUmWidth = al_get_bitmap_width(StrongEnemyImageUm) * 2;
	int StrongEnemyImageUmHeight = al_get_bitmap_height(StrongEnemyImageUm) * 2;
	int StrongEnemyImageDoisWidth = al_get_bitmap_width(StrongEnemyImageDois) * 2;
	int StrongEnemyImageDoisHeight = al_get_bitmap_height(StrongEnemyImageDois) * 2;
	int WeakShotHeight = al_get_bitmap_height(WeakShot) * 4;
	int WeakShotWidth = al_get_bitmap_width(WeakShot) * 4;
	int MediumShotHeight = al_get_bitmap_height(MediumShot) * 4;
	int MediumShotWidth = al_get_bitmap_width(MediumShot) * 4;
	int StrongShotHeight = al_get_bitmap_height(StrongShot) * 4;
	int StrongShotWidth = al_get_bitmap_width(StrongShot) * 4;
	int ExplosionImageWidth = al_get_bitmap_width(ExplosionImage) * 1.8;
	int ExplosionImageHeight = al_get_bitmap_height(ExplosionImage) * 1.8;
	int ShipDestroyedWidth =  al_get_bitmap_height(ShipDestroyed) * 5;
	int ShipDestroyedHeight =  al_get_bitmap_width(ShipDestroyed) * 1.2;
	int spaceImageWidth =  al_get_bitmap_width(spaceImage) * 2;
	int spaceImageHeight =  al_get_bitmap_height(spaceImage) * 5;
	int arrowImageWidth =  al_get_bitmap_width(arrowImage) * 2;
	int arrowImageHeight =  al_get_bitmap_height(arrowImage) * 2;
	int motherShipImageWidth =  al_get_bitmap_width(motherShipImage) * 2;
	int motherShipImageHeight =  al_get_bitmap_height(motherShipImage) * 2;

	/* Calcula a posicao inicial da matriz de modo que fique no centro da tela */
	int InitialXMatrix = (SCREEN_W - (ENEMY_COLUMNS * mediumEnemyImageUmWidth + (ENEMY_COLUMNS - 1) * ESPACAMENTO_ENTRE_ALIENS)) / 2;
	int InitialYMatrix = (SCREEN_H - (ENEMY_ROWS * mediumEnemyImageUmHeight + (ENEMY_ROWS - 1) * ESPACAMENTO_ENTRE_ALIENS)) / 2/2 + 50;


    ship* ship = ship_create(ShipImageHeight,ShipImageWidth, 2, SCREEN_W/2, SCREEN_H - 100, SCREEN_W, SCREEN_H);											
	if (!ship) return 1;
	EnemyBoard *enemyBoard = create_enemyMatrix(InitialXMatrix, InitialYMatrix, SCREEN_W, SCREEN_H, mediumEnemyImageUmWidth, mediumEnemyImageUmHeight);
	motherShip *motherShip = motherShip_create(motherShipImageHeight,motherShipImageWidth, -10, 80, SCREEN_W, SCREEN_H);	

	obstacle **obstacleMatrixUm = obstacle_create(SCREEN_W / 2 - (SCREEN_W / 2) / 2 - 20, SCREEN_H - 200, 20, 20, SCREEN_W, SCREEN_H);
	if (!obstacleMatrixUm) return 1;

	obstacle **obstacleMatrixDois = obstacle_create(SCREEN_W / 2 - 100, SCREEN_H - 200, 20, 20, SCREEN_W, SCREEN_H);
	if (!obstacleMatrixDois) return 1;

	obstacle **obstacleMatrixTres = obstacle_create(SCREEN_W / 2 + 100, SCREEN_H - 200, 20, 20, SCREEN_W, SCREEN_H);
	if (!obstacleMatrixTres) return 1;

	obstacle **obstacleMatrixQuatro = obstacle_create(SCREEN_W / 2 + (SCREEN_W / 2) / 2 - 20, SCREEN_H - 200, 20, 20, SCREEN_W, SCREEN_H);
	if (!obstacleMatrixQuatro) return 1;

	int frameCounter = 0;
	int switchImage = 0; 
	int ShipDead = 0;
	int EnemiesDead = 0;
	int LastScore;

	int playing = 1;
	int menu = 1;

	FILE *arquivo = fopen("last_score.bin", "rb");

	if (arquivo) {
		
		fread(&LastScore, sizeof(int), 1, arquivo);
		fclose(arquivo);
	}

	char scoreText[20];
	ALLEGRO_EVENT event;															
	al_start_timer(timer);

    while(playing)
	{																		
		al_wait_for_event(queue, &event);											
		
		if(menu){
			al_clear_to_color(al_map_rgb(0, 0, 0));

			sprintf(scoreText, "%d", LastScore);

			al_draw_text(small_font, al_map_rgb(51, 255, 0), 240, 25 , 0, scoreText);
			al_draw_text(small_font, al_map_rgb(255, 255, 255), 60, 25 , 0, "HIGHEST SCORE:");

			al_draw_text(font_title, al_map_rgb(255, 255, 255), SCREEN_W/2 - 60, 60 , 0, "SPACE");
			al_draw_text(font_title, al_map_rgb(51, 255, 0), SCREEN_W/2 - 100, 120 , 0, "INVADERS");

			al_draw_scaled_bitmap(weakEnemyImageUm, 0, 0, al_get_bitmap_width(weakEnemyImageUm), al_get_bitmap_height(weakEnemyImageUm),  SCREEN_W/2 - 100, SCREEN_H/2/2 + 50, weakEnemyImageUmWidth, weakEnemyImageUmHeight, 0);
			al_draw_scaled_bitmap(mediumEnemyImageUm, 0, 0, al_get_bitmap_width(mediumEnemyImageUm), al_get_bitmap_height(mediumEnemyImageUm),  SCREEN_W/2 - 100, SCREEN_H/2/2 + 110, mediumEnemyImageUmWidth, mediumEnemyImageUmHeight, 0);
			al_draw_scaled_bitmap(StrongEnemyImageUm, 0, 0, al_get_bitmap_width(StrongEnemyImageUm), al_get_bitmap_height(StrongEnemyImageUm),  SCREEN_W/2 - 90, SCREEN_H/2/2 + 170, StrongEnemyImageUmWidth, StrongEnemyImageUmHeight, 0);
			al_draw_scaled_bitmap(motherShipImage, 0, 0, al_get_bitmap_width(motherShipImage), al_get_bitmap_height(motherShipImage),  SCREEN_W/2 - 120, SCREEN_H/2/2 + 230, motherShipImageWidth, motherShipImageHeight, 0);
			
			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 30, SCREEN_H/2/2 + 50 , 0, " =    10 PTS");
			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 30, SCREEN_H/2/2 + 110 , 0, " =    20 PTS");
			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 30, SCREEN_H/2/2 + 170 , 0, " =    40 PTS");
			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 10, SCREEN_H/2/2 + 240 , 0, " =    ???");

			al_draw_text(small_font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 70, SCREEN_H/2 + 130, 0, "CONTROLS");				
																									
			al_draw_scaled_bitmap(spaceImage, 0, 0, al_get_bitmap_width(spaceImage), al_get_bitmap_height(spaceImage),SCREEN_W/2 - 80, SCREEN_H/2 + 170, spaceImageWidth, spaceImageHeight, 0);
			al_draw_scaled_bitmap(arrowImage, 0, 0, al_get_bitmap_width(arrowImage), al_get_bitmap_height(arrowImage),SCREEN_W/2 + 30, SCREEN_H/2 + 170, arrowImageWidth, arrowImageHeight, 0);
			al_draw_scaled_bitmap(arrowImage, 0, 0, al_get_bitmap_width(arrowImage), al_get_bitmap_height(arrowImage),SCREEN_W/2, SCREEN_H/2 + 170, arrowImageWidth, arrowImageHeight, ALLEGRO_FLIP_HORIZONTAL);

			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 150, SCREEN_H/2 + 230, 0, "PRESS ENTER TO PLAY");

			al_flip_display();	


			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) { 																			
				menu = 0; 																													
			} else if (event.type == 42) break;

		} else if(EnemiesDead){
			enemyBoard->destroyedEnemyTimer = 60;
		
			free_enemyBoard(enemyBoard);
			enemyBoard = create_enemyMatrix(InitialXMatrix, InitialYMatrix, SCREEN_W, SCREEN_H,mediumEnemyImageUmWidth, mediumEnemyImageUmHeight);
			
			if(ship->lifeCount < 5) ship->lifeCount++;
			EnemiesDead = 0;
			

		} else if(ShipDead) {

			al_draw_text(font_title, al_map_rgb(255, 255, 255),  SCREEN_W/2 - 100, SCREEN_H/2/2 + 60, 0, "GAME OVER");																					//Indique o modo de conclusão do programa
			al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2 - 130, SCREEN_H/2, 0, "press space to exit");																					//Indique o modo de conclusão do programa
			al_flip_display();																																												//Atualiza a tela

			if ((event.type == 10) && (event.keyboard.keycode == 75)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (event.type == 42) break; 					
			
		} else {
			if (event.type == 30){
        	
				if(!ship->destroyedShipTimer && !enemyBoard->destroyedEnemyTimer)
					update_position(ship,enemyBoard,motherShip);

				EnemiesDead = check_enemy_kill(ship,enemyBoard, ExplosionImage);
				ShipDead = check_ship_kill(ship,enemyBoard);			
				check_shipMother_kill(ship, motherShip);							
				draw_background(ship, ShipImage, font); 
				draw_obstacle(SquareCorner,ship,enemyBoard,obstacleMatrixUm,obstacleMatrixDois,obstacleMatrixTres,obstacleMatrixQuatro, DestroyedObstacleOne, DestroyedObstacleTwo, 
				DestroyedObstacleTree, DestroyedObstacleFour, DestroyedObstacleFive, DestroyedObstacleSix, DestroyedObstacleSeven, DestroyedObstacleEight, DestroyedObstacleNine,
				DestroyedCornerOne, DestroyedCornerTwo, DestroyedCornerTree);
			
				for (int i = 0; i < ENEMY_ROWS; i++) {
					for (int j = 0; j < ENEMY_COLUMNS; j++){
						if (switchImage == 0)
						{
							if(enemyBoard->enemyMatrix[i][j].difficulty == 3)
								al_draw_scaled_bitmap(StrongEnemyImageUm, 0, 0, al_get_bitmap_width(StrongEnemyImageUm), al_get_bitmap_height(StrongEnemyImageUm), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, StrongEnemyImageUmWidth, StrongEnemyImageUmHeight, 0);
							else if(enemyBoard->enemyMatrix[i][j].difficulty == 2)
								al_draw_scaled_bitmap(mediumEnemyImageUm, 0, 0, al_get_bitmap_width(mediumEnemyImageUm), al_get_bitmap_height(mediumEnemyImageUm), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, mediumEnemyImageUmWidth, mediumEnemyImageUmHeight, 0);
							else if(enemyBoard->enemyMatrix[i][j].difficulty == 1)
								al_draw_scaled_bitmap(weakEnemyImageUm, 0, 0, al_get_bitmap_width(weakEnemyImageUm), al_get_bitmap_height(weakEnemyImageUm), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, weakEnemyImageUmWidth, weakEnemyImageUmHeight, 0);
						}
						else
						{
							if(enemyBoard->enemyMatrix[i][j].difficulty == 3)
								al_draw_scaled_bitmap(StrongEnemyImageDois, 0, 0, al_get_bitmap_width(StrongEnemyImageDois), al_get_bitmap_height(StrongEnemyImageDois), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, StrongEnemyImageDoisWidth, StrongEnemyImageDoisHeight, 0);
							else if(enemyBoard->enemyMatrix[i][j].difficulty == 2)
								al_draw_scaled_bitmap(mediumEnemyImageDois, 0, 0, al_get_bitmap_width(mediumEnemyImageDois), al_get_bitmap_height(mediumEnemyImageDois), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, mediumEnemyImageDoisWidth, mediumEnemyImageDoisHeight, 0);
							else if(enemyBoard->enemyMatrix[i][j].difficulty == 1)
								al_draw_scaled_bitmap(weakEnemyImageDois, 0, 0, al_get_bitmap_width(weakEnemyImageDois), al_get_bitmap_height(weakEnemyImageDois), enemyBoard->enemyMatrix[i][j].x, enemyBoard->enemyMatrix[i][j].y, weakEnemyImageDoisWidth, weakEnemyImageDoisHeight, 0);

						}
					}
				}

				if (ship->destroyedShipTimer > 45)  {
					// Exibir a primeira imagem durante os primeiros 15 segundos
					al_draw_scaled_bitmap(ShipDestroyed, 0, 0, al_get_bitmap_width(ShipDestroyed), al_get_bitmap_height(ShipDestroyed), ship->x, ship->y, ShipDestroyedWidth, ShipDestroyedHeight, 0); 
					ship->destroyedShipTimer--;
				} 
				else if (ship->destroyedShipTimer > 30) {
					// Exibir a segunda imagem com a orientação invertida nos próximos 15 segundos
					al_draw_scaled_bitmap(ShipDestroyed, 0, 0, al_get_bitmap_width(ShipDestroyed), al_get_bitmap_height(ShipDestroyed), ship->x, ship->y, ShipDestroyedWidth, ShipDestroyedHeight, ALLEGRO_FLIP_HORIZONTAL); 
					ship->destroyedShipTimer--;
				}
				else if (ship->destroyedShipTimer > 15) {
					// Exibir a segunda imagem com a orientação invertida nos próximos 15 segundos
					al_draw_scaled_bitmap(ShipDestroyed, 0, 0, al_get_bitmap_width(ShipDestroyed), al_get_bitmap_height(ShipDestroyed), ship->x, ship->y, ShipDestroyedWidth, ShipDestroyedHeight, 0); 
					ship->destroyedShipTimer--;
				}
				else if (ship->destroyedShipTimer > 0) {
					// Exibir a segunda imagem com a orientação invertida nos próximos 15 segundos
					al_draw_scaled_bitmap(ShipDestroyed, 0, 0, al_get_bitmap_width(ShipDestroyed), al_get_bitmap_height(ShipDestroyed), ship->x, ship->y, ShipDestroyedWidth, ShipDestroyedHeight, ALLEGRO_FLIP_HORIZONTAL); 
					ship->destroyedShipTimer--;
				}
				else {
					// Quando o timer chegar a zero, exibir a imagem padrão da nave
					al_draw_scaled_bitmap(ShipImage, 0, 0, al_get_bitmap_width(ShipImage), al_get_bitmap_height(ShipImage), ship->x, ship->y, ShipImageWidth, ShipImageHeight, 0); 
				}		
				
				if(!ShipDead){
					for (bullet *index = ship->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_rectangle(index->x - BULLET_W, index->y - BULLET_H, index->x + BULLET_W, index->y + BULLET_H, al_map_rgb(255, 255, 255));								
						if (ship->gun->timer) ship->gun->timer--;	
					
					for (bullet *index = enemyBoard->gun->shots; index != NULL; index = (bullet*) index->next) 
					{
						if(index->type == 3)
							al_draw_scaled_bitmap(StrongShot, 0, 0, al_get_bitmap_width(StrongShot), al_get_bitmap_height(StrongShot), index->x, index->y, StrongShotWidth, StrongShotHeight, 0);
						else if(index->type == 2)
							al_draw_scaled_bitmap(MediumShot, 0, 0, al_get_bitmap_width(MediumShot), al_get_bitmap_height(MediumShot), index->x, index->y, MediumShotWidth, MediumShotHeight, 0);

						else if (index->type == 1)
							al_draw_scaled_bitmap(WeakShot, 0, 0, al_get_bitmap_width(MediumShot), al_get_bitmap_height(MediumShot), index->x, index->y, MediumShotWidth, MediumShotHeight, 0);

					}
					if (enemyBoard->gun->timer) enemyBoard->gun->timer--;	

					/* Imprime as explosoes na tela na posicao onde existia o inimigo que foi eliminado */
					for (int i = 0; i < MAX_EXPLOSIONS; i++){ 
						if(enemyBoard->explosions[i].lifespan > 0) {
							al_draw_scaled_bitmap(ExplosionImage, 0, 0, al_get_bitmap_width(ExplosionImage), al_get_bitmap_height(ExplosionImage), enemyBoard->explosions[i].x, enemyBoard->explosions[i].y, ExplosionImageWidth, ExplosionImageHeight, 0);
							enemyBoard->explosions[i].lifespan--;
						}
					}

					al_draw_scaled_bitmap(motherShipImage, 0, 0, al_get_bitmap_width(motherShipImage), al_get_bitmap_height(motherShipImage), motherShip->x, motherShip->y, motherShipImageWidth, motherShipImageHeight, 0);
					if(motherShip->timer) motherShip->timer--;
				}
	
				frameCounter++;

				if (frameCounter >= ENEMY_FRAME)
				{
					frameCounter = 0;
					switchImage = switchImage ^ 1;  // Alterna entre 0 e 1
				}	

				al_flip_display();								
			}
			else if ((event.type == 10) || (event.type == 12))
			{																																			
				if (event.keyboard.keycode == 82) joystick_left(ship->control);																													
				else if (event.keyboard.keycode == 83) joystick_right(ship->control);
				else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) joystick_fire(ship->control);																																																																																							
			}																																				
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;											
		}
	}

	arquivo = fopen("last_score.bin", "wb");

	if (arquivo) {
		if(ship->score >= LastScore)
			fwrite(&ship->score, sizeof(int), 1, arquivo);
		else	
			fwrite(&LastScore, sizeof(int), 1, arquivo);
		fclose(arquivo);
	}


	al_destroy_font(font);
	al_destroy_font(small_font);
	al_destroy_font(font_title);
	
	al_destroy_display(disp);														
	al_destroy_timer(timer);														
	al_destroy_event_queue(queue);
	
	al_destroy_bitmap(ShipImage);
	al_destroy_bitmap(weakEnemyImageUm);
	al_destroy_bitmap(weakEnemyImageDois);
	al_destroy_bitmap(mediumEnemyImageUm);
	al_destroy_bitmap(mediumEnemyImageDois);
	al_destroy_bitmap(StrongEnemyImageUm);
	al_destroy_bitmap(StrongEnemyImageDois);
	al_destroy_bitmap(WeakShot);
	al_destroy_bitmap(MediumShot);
	al_destroy_bitmap(StrongShot);
	al_destroy_bitmap(SquareCorner);
	al_destroy_bitmap(ExplosionImage);
	al_destroy_bitmap(ShipDestroyed);
	al_destroy_bitmap(DestroyedObstacleOne);
	al_destroy_bitmap(DestroyedObstacleTwo);
	al_destroy_bitmap(DestroyedObstacleTree);
	al_destroy_bitmap(DestroyedObstacleFour);
	al_destroy_bitmap(DestroyedObstacleFive);
	al_destroy_bitmap(DestroyedObstacleSix);
	al_destroy_bitmap(DestroyedObstacleSeven);
	al_destroy_bitmap(DestroyedObstacleEight);
	al_destroy_bitmap(DestroyedObstacleNine);
	al_destroy_bitmap(DestroyedCornerOne);
	al_destroy_bitmap(DestroyedCornerTwo);
	al_destroy_bitmap(DestroyedCornerTree);
	al_destroy_bitmap(motherShipImage);

	ship_destroy(ship);
	free_enemyBoard(enemyBoard);
	obstacle_destroy(&obstacleMatrixUm);
	obstacle_destroy(&obstacleMatrixDois);
	obstacle_destroy(&obstacleMatrixTres);
	obstacle_destroy(&obstacleMatrixQuatro);
	if(motherShip)
		free(motherShip);

	return 0;
}