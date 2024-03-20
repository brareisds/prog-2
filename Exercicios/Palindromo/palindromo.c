#include <stdio.h>
#include <string.h>
#include "deque.h"
#define TAM 255

/*void imprime_deque(deque_t *deque)
{
    while(deque->ini)
    {
        printf("deque->ini->chave: %c\n", deque->ini->chave );
        deque->ini = deque->ini->prox;
    }
}*/

int main()
{
    deque_t *d;
    int i, eh_palindromo, tamanho;
    char palavra[TAM], caractere, ini, fim;
  
    fgets(palavra, TAM, stdin);
    d = cria_deque();

    tamanho = strlen(palavra)-1;

    /*Percorre cada caractere na palavra e o adiciona ao deque*/
    for (i = 0; i < tamanho; i++) 
    {
        caractere = palavra[i];
        inserir_deque_fim(d, caractere);
    }

    eh_palindromo = 1;
    while(!deque_vazio(d) && eh_palindromo)
    {
        
        remover_deque_ini(d,&ini);

        /* Verifica se o deque ficou vazio e atualiza o fim. 
        Se o deque ficou vazio, significa que so restou uma letra e, portanto, eh palindromo */
        if(!deque_vazio(d))
            remover_deque_fim(d,&fim);
        else
            fim = ini;
        
        if(ini != fim)
            eh_palindromo = 0;
    }
    
    if(eh_palindromo)
        printf("Eh palindromo\n");
    else 
        printf("Nao eh palindromo\n");

    
    destroi_deque(&d);
    
    return 0;
}
