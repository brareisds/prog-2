
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int altura_mapa(FILE *mapa)
{
    int linhas;
    fscanf(mapa,"%d", &linhas);
    /* Pega o espaço */
    fgetc(mapa);

    return linhas;
}

int largura_mapa(FILE *mapa)
{
    int colunas;
    fscanf(mapa,"%d",&colunas);
    /* Pega o \n */
    fgetc(mapa);

    return colunas;

}

int numero_celulas(int altura, int largura)
{
    return altura * largura;
}

char ** matriz_mapa(FILE *arquivo, int altura, int largura)
{
    char buffer[largura + 2], **mapa;

    mapa = malloc(sizeof(char*) * altura);

    for(int i = 0; i < altura; i++)
    {
        fread(buffer, sizeof(char), largura+1, arquivo);
        buffer[largura + 1] = '\0'; 

        /* Aloca a largura */
        mapa[i] = strdup(buffer);
    }

    return mapa;
}

void liberaMemoria(char **mapa, int altura)
{
    for (int i = 0; i < altura; i++)
        free(mapa[i]);
    free(mapa);
}

