#include <stdlib.h>
#include "libfila.h"

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
fila_t *cria_fila()
{
    fila_t *f;

    if(!(f = malloc(sizeof(fila_t))))
        return NULL;

    f->ini = NULL;
    f->fim = NULL;
   
   return f;
}

noticia_t *cria_noticia()
{
    noticia_t *nova_noticia;

    if(!(nova_noticia = malloc(sizeof(noticia_t))))
        return 0;

    nova_noticia->idade = 0;
    nova_noticia->prox = NULL;

    return nova_noticia;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (fila_t **fila)
{
    noticia_t *aux;

    while((*fila)->ini)
    {
        aux = (*fila)->ini;
        (*fila)->ini = aux->prox;
        free(aux);
    }

    free(*fila);
    (*fila) = NULL;
}

/* 
 * Insere dado no final da fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (fila_t *fila, noticia_t *noticia){

    noticia->prox = NULL;

    /* Se a fila estiver vazia, a ini e a fim 
     apontarão para o novo noticia.*/
    if(fila_vazia(fila))
    {
        fila->ini = noticia;
        fila->fim = noticia;

        return 1;
    }

    /* Caso contrario, o novo noticia eh inserido apos o ultimo noticia e somente 
     o ponteiro da fim eh atualizado para o final da fila*/
    fila->fim->prox = noticia;
    fila->fim = noticia; 
 
    return 1;
}

/* 
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro dado. Retorna 1 em caso de sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (fila_t *fila)
{
    noticia_t *aux;

    if(fila_vazia(fila))
        return 0; 

    /* Antes de dar free no aux que armazena o primeiro noticia que sera removido da fila,
      o inicio da mesma eh atualizado para o noticia seguinte e o dado eh guardado */
    aux = fila->ini;
    fila->ini = aux->prox;

    free(aux);

    return 1;

}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (fila_t *fila){

    if(!fila->ini || !fila->fim)
        return 1;

    return 0;
}