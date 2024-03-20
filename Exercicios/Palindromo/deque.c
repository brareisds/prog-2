#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

/* 
 * Cria e retorna um novo deque.
 * Retorna NULL em caso de erro de alocação.
*/
deque_t *cria_deque ()
{
    deque_t *d;

    if(!(d=malloc(sizeof(deque_t))))
        return NULL;

    d->ini = NULL;
    d->fim = NULL;
    d->tamanho = 0;

    return d;
}

/* Desaloca toda memoria do deque e faz o deque receber NULL. */
void destroi_deque (deque_t **deque)
{
    elemento_t *aux;

    while((*deque)->ini)
    {
        aux = (*deque)->ini;
        (*deque)->ini = aux->prox;
        free(aux);
    }

    free(*deque);
    *deque =  NULL;
}

/* 
 * Insere caractere no fim do deque. 
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
*/
int inserir_deque_fim (deque_t *deque, char caractere)
{
    elemento_t *novo;

    if(!(novo=malloc(sizeof(elemento_t))))
        return 0;
    
    
    novo->chave = caractere;
    novo->prox = NULL;
    novo->ant = deque->fim;

    /* Se o deque estiver vazio o inicio e o fim apontarao para o mesmo nodo */
    if(deque_vazio(deque))
        deque->ini = novo;
    else    
        deque->fim->prox = novo;
    
    deque->fim = novo;
    deque->tamanho++;

    return 1;
}

/* 
 * Insere caractere no inicio do deque. 
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
*/
int inserir_deque_ini (deque_t *deque, char caractere)
{
    elemento_t *novo;

    if(!(novo=malloc(sizeof(elemento_t))))
        return 0;

    novo->chave = caractere;
    novo->ant = NULL;
    novo->prox = deque->ini;
    
    /* Se o deque estiver vazio o inicio e o fim apontarao para o mesmo nodo */
    if(deque_vazio(deque))
        deque->fim = novo;
    else
        deque->ini->ant = novo;

    deque->ini = novo;

    deque->tamanho++;
    
    return 1;
}

/* Remove o ultimo elemento do deque e o retorna pelo parametro. 
 * Retorna 1 em caso de sucesso e 0 caso contrario */
int remover_deque_fim (deque_t *deque, char *caractere)
{
    elemento_t *aux;

    if(deque_vazio(deque))
        return 0;


    aux = deque->fim;
    *caractere = aux->chave;

    if(deque->ini == deque->fim)
        deque->ini = NULL;
    else
        aux->ant->prox = aux->prox;

    deque->fim = aux->ant;
    deque->tamanho--;

    free(aux);

    return 1;
}

/* Remove o primeiro elemento do deque e o retorna pelo parametro. 
 * Retorna 1 em caso de sucesso e 0 caso contrario */
int remover_deque_ini (deque_t *deque, char *caractere)
{
    elemento_t *aux;

    if(deque_vazio(deque))
        return 0;


    aux = deque->ini;
    *caractere = aux->chave;

    /* Verifica se só existe um elemento no deque. */
    if(deque->ini == deque->fim)
        deque->fim = NULL;
    else
        aux->prox->ant = aux->ant;

    deque->ini = aux->prox;
    deque->tamanho--;

    free(aux);

    return 1;
}
 

/* Retorna o numero de elementos da deque. */
int deque_tamanho (deque_t *deque)
{
    return deque->tamanho;
}

/* Retorna 1 se deque vazio, 0 em caso contrario. */ 
int deque_vazio (deque_t *deque)
{
    if(deque_tamanho(deque)==0)
        return 1;

    return 0;
}
