#define TITULO 32
#define TEXT 512

typedef struct noticia {
    int idade;
    char titulo[TITULO];
    char texto[TEXT];
    struct noticia *prox;
} noticia_t;

typedef struct fila{
    noticia_t *ini;
    noticia_t *fim;
} fila_t;

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
fila_t *cria_fila ();

noticia_t *cria_noticia();

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (fila_t **fila);

/* 
 * Insere dado no final da fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (fila_t *fila, noticia_t *noticia);

/* 
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro dado. Nao confundir com o retorno da funcao.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (fila_t *fila);
 

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (fila_t *fila);
