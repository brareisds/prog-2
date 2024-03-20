#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"

shot_sentinel* create_shotlist(void){
	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
	if (list == NULL) return NULL;
	list->first = NULL;
	list->last  = NULL;
	return list;
}

shot* remove_shot(shot* current, shot* previous, shot_sentinel *list){
/*	Remove os tiros da lista */
	shot* prox = NULL;

	if(previous)
		previous->next = current->next;

	if(current == list->first)
		list->first = current->next;
	
	if(current == list->last)
		list->last = previous;

	prox = current->next;
	free(current);
	current = NULL;

	return prox;
}

void clean_shots(shot_sentinel *list){
	shot *p = (shot*) list->first;
	shot *q = NULL;
	while (p != NULL)
		p = remove_shot(p, q, list);
}

void update_shots(space *board, shot_sentinel *list)
{
    shot *aux = list->first;
    shot *previous = NULL, *next_shot;

	while (aux)
	{
		/* Atualiza a posição vertical do tiro */
		aux->position_y++;

		next_shot = aux->next;
		/* Verifica se o tiro saiu do tabuleiro e remove o tiro da lista */
		if (aux->position_y > board->max_y) 
			remove_shot(aux, previous, list);
		else
			previous = aux;
		aux = next_shot;
	}
}


shot* straight_shot(space *board, shot_sentinel *list, enemy *shooter)
{
    shot *novo_tiro;

    novo_tiro = malloc(sizeof(shot));
    if (!novo_tiro) {
        printf("Falha de alocação de memoria\n");
        return NULL;
    }

    novo_tiro->position_x = shooter->position_x;
    novo_tiro->position_y = shooter->position_y + 1;
	novo_tiro->next = NULL;
	novo_tiro->trajectory = 0;

    if (!list->first) {
        list->first = novo_tiro;
        list->last = novo_tiro;
    } else {
        list->last->next = novo_tiro;
        list->last = novo_tiro;
    }

    return novo_tiro;
}


/* Esta função adiciona um inimigo/alien ao tabuleiro, fazendo a sua alocação.*/
int add_enemy(space *board, int position_y, int position_x){

	/* Verifica se as coordenadas estao dentro do limite do tabuleiro */
	if ((position_y <= 0) || (position_x <= 0) || (position_x > board->max_x) || (position_y > board->max_y)) return 0;

	enemy *inimigo;

	inimigo = (enemy *) malloc(sizeof(enemy));
	if(!inimigo)
	{
		printf("Falha de alocação de memoria\n");
		return 0;
	}

	inimigo->position_y = position_y;
	inimigo->position_x = position_x;

	board->map[position_y][position_x].type = ENEMY;
	board->map[position_y][position_x].entity = inimigo;

	return 1;
}

/* Remove um inimigo/alien ao tabuleiro, realizando a sua desalocação.*/
int remove_enemy(space *board, int position_y, int position_x)
{
	if(board->map[position_y][position_x].type != ENEMY || (position_y <= 0) || 
	(position_x <= 0) || (position_x > board->max_x) || (position_y > board->max_y))
		return 0;

	/* Libera a memoria do inimigo e atualiza o tabuleiro */
	free(board->map[position_y][position_x].entity);

	
	board->map[position_y][position_x].type = 0;
	board->map[position_y][position_x].entity = NULL;

	return 1;
}
