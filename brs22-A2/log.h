#include "arff.h"
#ifndef __LOG__
#define __LOG__

typedef struct{
    char *origem;
    int contador;
} endereco;

typedef struct{
    int soma;
    int quant;
} info_media;


void relatorio_ataques(FILE *arff, atributo *atributos,int quantidade);
void relatorio_enderecos_maliciosos(FILE *arff, atributo *atributos,int quantidade);
void relatorio_media_tamanho(FILE *arff, atributo *atributos, int quantidade);
void lista_negra(FILE *arff, atributo *atributos);


#endif