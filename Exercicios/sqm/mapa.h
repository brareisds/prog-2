#ifndef __MAPA__
#define __MAPA__


int altura_mapa(FILE *mapa);

int largura_mapa(FILE *mapa);

int numero_celulas(int altura, int largura);

char ** matriz_mapa(FILE *arquivo, int altura, int largura);

void liberaMemoria(char **mapa, int altura);

#endif