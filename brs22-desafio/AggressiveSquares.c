//Compilação: gcc AggressiveSquares.c Square.c Joystick.c Bullet.c Pistol.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags) // Nova linha de compilação (!)

#include <allegro5/allegro5.h>																																												
#include <allegro5/allegro_font.h>																																											
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>																	// Biblioteca que permite adicionar imagens (!)													

#include <stdio.h> 																					// Biblioteca para adicionar saida de erro na tela (!)

#include "Square.h"																																															

#define X_SCREEN 320																																														 
#define Y_SCREEN 320																																														 

unsigned char collision_2D(square *element_first, square *element_second){													
	
	if ((((element_second->y-element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y+element_first->side/2 >= element_second->y-element_second->side/2)) || 				
		((element_first->y-element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y+element_second->side/2 >= element_first->y-element_first->side/2))) && 	
		(((element_second->x-element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x+element_first->side/2 >= element_second->x-element_second->side/2)) || 				
		((element_first->x-element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x+element_second->side/2 >= element_first->x-element_first->side/2)))) return 1;			
	else return 0;																																															
}

unsigned char check_kill(square *killer, square *victim){																																					

	bullet *previous = NULL;
	for (bullet *index = killer->gun->shots; index != NULL;){																																			
		if ((index->x >= victim->x - victim->side/2) && (index->x <= victim->x + victim->side/2) && 																								
		   (index->y >= victim->y - victim->side/2) && (index->y <= victim->y + victim->side/2)){																											
			if (previous){																			// Remove o projetil da lista encadeada de tiros do killer (!)																									
				previous->next = index->next;														// (!)																									
				bullet_destroy(index);																// (!)																									
				index = (bullet*) previous->next;													// (!)																										
			}																						// (!)
			else {																					// (!)																										
				killer->gun->shots = (bullet*) index->next;											// (!)																										
				bullet_destroy(index);																// (!)																										
				index = killer->gun->shots;															// (!)																										
			}																						// (!)
		
			victim->lifeCount--; 																	// Decrementa a vida da vitima (!)
		}
		else{																																																
			previous = index;																		// (!)																																							
			index = (bullet*) index->next;															// (!)																																							
		}
	}

	if(!victim->lifeCount) 																			// Verifica se as vidas do jogador acabaram (!)
		return 1;
	else
		return 0;																																																
}

void update_bullets(square *player){																																										
	
	bullet *previous = NULL;																																												
	for (bullet *index = player->gun->shots; index != NULL;){																																				
		if (!index->trajectory) index->x -= BULLET_MOVE;																																					
		else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			
		
		if ((index->x < 0) || (index->x > X_SCREEN)){																																						
			if (previous){																																													
				previous->next = index->next;																																								
				bullet_destroy(index);																																										
				index = (bullet*) previous->next;																																							
			}
			else {																																														
				player->gun->shots = (bullet*) index->next;																																					
				bullet_destroy(index);																																										
				index = player->gun->shots;																																									
			}
		}
		else{																																															
			previous = index;																																												
			index = (bullet*) index->next;																																									
		}
	}
}

void update_position(square *player_1, square *player_2, int NewUpperYScreen){									// Um novo parâmetro foi adicionado para indicar o novo limite superior da tela (!)																												

	if (player_1->control->left){																																											
		square_move(player_1, 1, 0, X_SCREEN, Y_SCREEN,NewUpperYScreen); 										// Chamadas das funcoes com o novo parametro (!)																																	
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 0, X_SCREEN, Y_SCREEN,NewUpperYScreen); // (!)																			
	}
	if (player_1->control->right){																																											
		square_move(player_1, 1, 1, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																				
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 1, X_SCREEN, Y_SCREEN,NewUpperYScreen);	// (!)																											
	}
	if (player_1->control->up) {																																											
		square_move(player_1, 1, 2, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																				
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 2, X_SCREEN, Y_SCREEN,NewUpperYScreen);	// (!)																										
	}
	if (player_1->control->down){																																											
		square_move(player_1, 1, 3, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																			
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 3, X_SCREEN, Y_SCREEN, NewUpperYScreen); // (!)																											
	}

	if (player_2->control->left){																																											
		square_move(player_2, 1, 0, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																					
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 0, X_SCREEN, Y_SCREEN, NewUpperYScreen); // (!)																										
	}
	
	if (player_2->control->right){ 																																											
		square_move(player_2, 1, 1, X_SCREEN, Y_SCREEN, NewUpperYScreen);										// (!)																								
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 1, X_SCREEN, Y_SCREEN, NewUpperYScreen); // (!)																										
	}
	
	if (player_2->control->up){																																												
		square_move(player_2, 1, 2, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 2, X_SCREEN, Y_SCREEN, NewUpperYScreen); // (!)																												
	}
	if (player_2->control->down){																																											
		square_move(player_2, 1, 3, X_SCREEN, Y_SCREEN, NewUpperYScreen); 										// (!)																																			
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 3, X_SCREEN, Y_SCREEN, NewUpperYScreen); // (!)																												
	}
	if (player_1->control->fire){																																											
		if (!player_1->gun->timer){																																											
			square_shot(player_1); 																																											
			player_1->gun->timer = PISTOL_COOLDOWN;																																							
		} 
	}
	if (player_2->control->fire){																																											
		if (!player_2->gun->timer){																																											
			square_shot(player_2);																																											
			player_2->gun->timer = PISTOL_COOLDOWN;																																							
		}
	}
	update_bullets(player_1);																																												
	update_bullets(player_2);																																												
}


void checkPlayerLives(square *player_1, square *player_2, ALLEGRO_BITMAP *RedHeart, ALLEGRO_BITMAP *BlueHeart, int heartWidthRed, int heartHeightRed, int heartWidthBlue, int heartHeightBlue){ // Nova funcao responsavel por desenhar os coracoes na tela com base na quantidade de vidas dos jogadores  (!)

	/* Posição inicial no lado esquerdo */ 
	int startXRed = 10;  																																	// (!)
	/* Posição inicial no lado direito */
	int startXBlue = X_SCREEN - 30; 																														// (!)

	/* Espaçamento entre os coracoes */
	int heartSpacing = 20; 																																	// (!)

	for(int i = 0; i < player_1->lifeCount; i++){ 																											// (!)
		int x = startXRed + i * heartSpacing;  																												// Calcula a posição X com base no índice (!)
		al_draw_scaled_bitmap(RedHeart, 0, 0, al_get_bitmap_width(RedHeart), al_get_bitmap_height(RedHeart), x, 10, heartWidthRed, heartHeightRed, 0); 		// (!)
	}
		
	for (int i = 0; i < player_2->lifeCount; i++) { 																										// (!)
    	int x = startXBlue - i * heartSpacing;  																											// Calcula a posição X com base no índice (!)
    	al_draw_scaled_bitmap(BlueHeart, 0, 0, al_get_bitmap_width(BlueHeart), al_get_bitmap_height(BlueHeart), x, 10, heartWidthBlue, heartHeightBlue, 0); // (!)
	}
	
}


int main(){
	
	al_init();																																																
	al_init_primitives_addon();																																												
	al_install_keyboard();																																													
	al_init_image_addon(); 																				// Prepara o ambiente de trabalho do Allegro 5 para lidar com imagens que serao adicionada. (!)

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																							
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();		
	ALLEGRO_FONT* font = al_create_builtin_font();																																				
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																			

	ALLEGRO_BITMAP *RedHeart = NULL; 																	// Declara os ponteiros para as imagens que irao representar visualmente a vida dos jogadores (!)
	ALLEGRO_BITMAP *BlueHeart = NULL; 																	// (!)
	
	al_register_event_source(queue, al_get_keyboard_event_source());																									
	al_register_event_source(queue, al_get_display_event_source(disp));																																		
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		

	square* player_1 = square_create(20, 1, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															
	if (!player_1) return 1;																																												
	square* player_2 = square_create(20, 0, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													
	if (!player_2) return 2;																																												

	/* Inicializa as imagens */
	RedHeart = al_load_bitmap("RedHeart.bmp"); 															// Carrega as imagens de coracao na tela (!)
	BlueHeart = al_load_bitmap("BlueHeart.bmp"); 														// (!)

	if (!RedHeart) {
		fprintf(stderr, "Failed to load RedHeart image.\n");
		return 1;
	}
	if (!BlueHeart) {
		fprintf(stderr, "Failed to load BlueHeart image.\n");
		return 1;
	}

	/* Ajusta a proporção da imagem */
	int heartWidthRed = al_get_bitmap_width(RedHeart) / 60;											 //  Ajustam as proporcoes das imagens de coracoes na tela (!)
	int heartWidthBlue = al_get_bitmap_width(BlueHeart) / 60; 										// (!)
	int heartHeightRed = al_get_bitmap_height(RedHeart) / 60; 										// (!)
	int heartHeightBlue = al_get_bitmap_height(BlueHeart) / 60; 									// (!)

	/* Ajusta o tamanho da tela para que os coracoes nao sejam sobrepostos 							
	com base na maior proporcao de altura */
	int NewUpperYScreen; 																			//  Variavel usada para definir o novo limite superior da tela baseado na altura dos coracoes (!)
	if(heartHeightRed >= heartHeightBlue)															// (!)
		NewUpperYScreen = heartHeightRed + 10;														// (!)
	else																							// (!)
		NewUpperYScreen = heartHeightBlue + 10;														// (!)

	ALLEGRO_EVENT event;																																													
	al_start_timer(timer);																																													
	unsigned char p1k = 0, p2k = 0;																																																	
	while(1){																																																
		al_wait_for_event(queue, &event);																																									

		if (p1k || p2k){																																													 
			al_clear_to_color(al_map_rgb(0, 0, 0));																																							
			if (p2k && p1k) al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 40, Y_SCREEN/2-15, 0, "EMPATE!");																					
			else if (p2k) al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR 1 GANHOU!");																				
			else if (p1k) al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR 2 GANHOU!");																				
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 150, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA JOGAR NOVAMENTE");		// Modifica as mensagens de conclusao do jogo (!)																			
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+25, 0, "OU PRESSIONE ESC PARA SAIR"); 				// (!)
			al_flip_display();																																												

			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) { 																				// Usa a tecla ESPAÇO para jogar novamente, inicializando os jogadores e suas estruturas  (!)
				p1k = 0; 																													// (!)
				p2k = 0; 																													// (!)

				/* Libera a memória alocada para os jogadores */ 
				square_destroy(player_1); 																									// (!)
				square_destroy(player_2); 																									// (!)

				/* Recria os jogadores*/
				player_1 = square_create(20, 1, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN); 														// (!)
				player_2 = square_create(20, 0, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN); 												// (!)
			}

			else if ((event.type == 10) && (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) break;											// Usa agora a tecla ESC para encerrar do jogo (!)																								
			else if (event.type == 42) break; 																																							
		} else {																																															
			if (event.type == 30){																																											
				update_position(player_1, player_2, NewUpperYScreen);																		// O novo parâmetro "NewUpperYScreen" eh adicionado na chamada de função (!)																												
				p1k = check_kill(player_2, player_1);																																						
				p2k = check_kill(player_1, player_2);																																						
				al_clear_to_color(al_map_rgb(0, 0, 0));	

    			al_draw_line(0, NewUpperYScreen + 5, X_SCREEN, NewUpperYScreen + 5, al_map_rgb(54,54,54), 1); 								// Desenha uma linha horizontal no inicio da tela para representar o novo limite superior dos jogadores (!)
				checkPlayerLives(player_1, player_2, RedHeart, BlueHeart, heartWidthRed, heartHeightRed, heartWidthBlue, heartHeightBlue); // Desenha os coracoes na parte superior da tela com base na qntd de vidas dos jogadores (!)

				al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255, 0, 0));					
				al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0, 0, 255));					
	    		for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));								
	    		if (player_1->gun->timer) player_1->gun->timer--;																																			
	    		for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));								
	    		if (player_2->gun->timer) player_2->gun->timer--; 																																			
	    		al_flip_display();																																											
			}
			else if ((event.type == 10) || (event.type == 12)){						
				if (event.keyboard.keycode == 1) joystick_left(player_1->control);																															
				else if (event.keyboard.keycode == 4) joystick_right(player_1->control);																													
				else if (event.keyboard.keycode == 23) joystick_up(player_1->control);																														
				else if (event.keyboard.keycode == 19) joystick_down(player_1->control);																													
				else if (event.keyboard.keycode == 82) joystick_left(player_2->control);																													
				else if (event.keyboard.keycode == 83) joystick_right(player_2->control);																													
				else if (event.keyboard.keycode == 84) joystick_up(player_2->control);																														
				else if (event.keyboard.keycode == 85) joystick_down(player_2->control);																													
				else if (event.keyboard.keycode == 3) joystick_fire(player_1->control);																																	
				else if (event.keyboard.keycode == 218) joystick_fire(player_2->control);																													
			}																																			
			else if (event.type == 42) break;		
		}																																																	
		
	}

	al_destroy_font(font);																																													
	al_destroy_display(disp);																																												
	al_destroy_timer(timer);																																												
	al_destroy_event_queue(queue);																																											
	al_destroy_bitmap(RedHeart);
	al_destroy_bitmap(BlueHeart);
	square_destroy(player_1);																																												
	square_destroy(player_2);																																												

	return 0;
}