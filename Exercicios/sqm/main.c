#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

int main()
{
    int altura, largura, qntd_celulas;
    char **mapa;

    FILE *arquivo = fopen("mapa.txt","r");
    if(!arquivo)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        return 1;
    }

    altura = altura_mapa(arquivo);
    largura = largura_mapa(arquivo);
    qntd_celulas = numero_celulas(altura,largura);

    printf("altura: %d\n", altura);
    printf("largura: %d\n", largura);
    printf("quant celulas: %d\n", qntd_celulas);

    mapa = matriz_mapa(arquivo, altura, largura);

    /* Imprime o mapa */
    for(int i = 0; i < altura; i++)
        printf("%s",mapa[i]);
    
    /* Libera a memória alocada para a matriz */
    liberaMemoria(mapa,altura);

    fclose(arquivo);

    return 0;
}