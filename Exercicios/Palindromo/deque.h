typedef struct elemento {
    char chave;
    struct elemento *prox;
    struct elemento *ant;
} elemento_t;

typedef struct deque {
    struct elemento *ini;
    struct elemento *fim;
    int tamanho;
} deque_t;

/* 
 * Cria e retorna um novo deque.
 * Retorna NULL em caso de erro de alocação.
*/
deque_t *cria_deque ();

/* Desaloca toda memoria do deque e faz o deque receber NULL. */
void destroi_deque (deque_t **deque);

/* 
 * Insere chave no fim do deque. 
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
*/
int inserir_deque_fim (deque_t *deque, char chave);

/* 
 * Insere chave no inicio do deque. 
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
*/
int inserir_deque_ini (deque_t *deque, char chave);

/* 
 * Remove os extremos e retorna os elementos 
 * nos parametros chave1 e chave2. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da deque estar vazia.
*/
int remover_deque_fim (deque_t *deque, char *fim);

int remover_deque_ini (deque_t *deque, char *ini);
 
/* Retorna o numero de elementos da deque, que pode ser 0. */
int deque_tamanho (deque_t *deque);

/* Retorna 1 se deque vazio, 0 em caso contrario. */ 
int deque_vazio (deque_t *deque);

