#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#define TAM_ATRIBUTO 1025
#define TAM_DADOS 2049

/* Gera um relatorio de todos os ataques ocorridos e o numero de ocorrencias do conjunto de dados*/
void relatorio_ataques(FILE *arff, atributo *atributos, int quantidade){

    char buffer_atributos[TAM_ATRIBUTO], buffer_dados[TAM_DADOS], *token, *dados;
    int *ataques, qntd_categorias, atributo_de_ataque = -1, fim_arquivo;

    FILE *arquivo;
    arquivo = fopen("R_ATAQUES.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    /* Quantidade de pacotes que indicam ataque */
    qntd_categorias = 5;

    if(!(ataques = malloc(qntd_categorias * sizeof(int))))
    {
        fprintf(stderr, "Falha de alocacao de memoria!\n");
        exit(EXIT_FAILURE);
    }

    
    /* Inicializa os contadores dos ataques */
    for(int k = 0; k < qntd_categorias; k++)
        ataques[k] = 0;

    /* Posiciona o ponteiro de leitura para antes do @data */
    do {
        memset(buffer_atributos, 0, sizeof(buffer_atributos));

        fscanf(arff,"%[^\n]",buffer_atributos);
        fgetc(arff);

        if(!buffer_atributos[0])
            continue;

        token = strtok(buffer_atributos, " ");

    } while(token && strcmp(token, "@data") != 0);


    fim_arquivo = fscanf(arff,"%[^\n]",buffer_dados);
    fgetc(arff);

    if(fim_arquivo != -1)
    {
        do{
            int i = 0, j;
            memset(buffer_dados, 0, sizeof(buffer_dados));

            fscanf(arff,"%[^\n]",buffer_dados);
            fgetc(arff);

            /* Pula a linha que contem @data e possiveis linhas em branco */
            if (!buffer_dados[0] || strcmp(buffer_dados, "@data") == 0)
                continue;
            
            dados = strtok(buffer_dados, ","); 

            
            while(dados && i < quantidade)
            {
                /* Verifica se o rotulo indica uma ocorrencia de ataque */
                if(dados && strcmp(atributos[i].rotulo, "PKT_CLASS") == 0 && strcmp(dados, "Normal"))
                {
                    /* Soma a quantidade de ocorrencias do pacote especifico. O pacote [j+1] corresponde ao ataque[j] */
                    j = 0;
                    while(j < qntd_categorias-1 && atributos[i].categorias[j])
                    {
                        if(strcmp(dados, atributos[i].categorias[j+1]) == 0){
                            ataques[j]++;
                        }
                        j++;
                    }
                    atributo_de_ataque = i;
                }

                dados = strtok(NULL, ",");

                i++;
            } 

            /* Testa se a quantidade de atriutos e de dados sao compativeis */
            if(dados)
            {
                printf("A quantidade de dados eh maior que a quantidade de atributos\n");
                fclose(arquivo);
                return;
            } else if(i < quantidade)
            {
                printf("A quantidade de dados eh menor que a quantidade de atributos\n");
                fclose(arquivo);
                return;
            }

        }while(!feof(arff));
   
        /* Percorre o vetor de ataques relacionando o nome do ataque correspondente presente no atributo PKT_CLASS*/
        if(atributo_de_ataque == -1){
            printf("Arquivo nao possui atributo PKT_CLASS\n");
        } else
            for(int indice = 0; indice < qntd_categorias-1; indice++)
                fprintf(arquivo,"%s;%d\n",atributos[atributo_de_ataque].categorias[indice+1], ataques[indice]); 

    } else
        printf("O arquivo ainda nao contem os dados da seção @data\n");

    fclose(arquivo);

    free(ataques);
}


void relatorio_enderecos_maliciosos(FILE *arff, atributo *atributos, int quantidade)
{
    endereco *enderecos = NULL;
    char *end_origem, *pacote;
    char buffer_atributos[TAM_ATRIBUTO], buffer_dados[TAM_DADOS], *token, *dados, classificacao[50];
    int qnt_enderecos = 0, fim_arquivo;

    FILE *arquivo;
    arquivo = fopen("R_ENTIDADES.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
   
    /* Posiciona o ponteiro de leitura para antes do @data */
    do {
        memset(buffer_atributos, 0, sizeof(buffer_atributos));

        fscanf(arff,"%[^\n]",buffer_atributos);
        fgetc(arff);

        if(!buffer_atributos[0])
            continue;

        token = strtok(buffer_atributos, " ");

    } while(token && strcmp(token, "@data") != 0);

    /* Descarta a primeira linha @data  e verifica se chegou ao fim do arquivo */
    fim_arquivo = fscanf(arff,"%[^\n]",buffer_dados);
    fgetc(arff);
    int linha = 0;

    if(fim_arquivo != -1)
    {
        do{
            int i = 0;
            memset(buffer_dados, 0, sizeof(buffer_dados));

            fscanf(arff,"%[^\n]",buffer_dados);
            fgetc(arff);
        
            dados = strtok(buffer_dados, ","); 
            
            while(dados && i < quantidade)
            {
                if(strcmp(atributos[i].rotulo, "SRC_ADD") == 0)
                    end_origem = dados;
                if (strcmp(atributos[i].rotulo, "PKT_CLASS") == 0)
                    pacote = dados;
                
                dados = strtok(NULL, ",");

                if (!buffer_dados[0])
                    continue;

                i++;
            } 

            if(dados){
                printf("A quantidade de dados eh maior que a quantidade de atributos\n");
                fclose(arquivo);
                return;
            } else if(i < quantidade)
            {
                fclose(arquivo);
                printf("A quantidade de dados eh menor que a quantidade de atributos\n");
                return;
            }
            
            
            int encontrou_origem = 0;

            for (int i = 0; i < qnt_enderecos; i++)
            {
                if(strcmp(enderecos[i].origem, end_origem) == 0)
                {
                    /* Soma somente seo pacote for diferente de normal. */
                    if(strcmp(pacote, "Normal"))
                        enderecos[i].contador++;

                    encontrou_origem = 1;
                    break;
                }
            }

            
            if(!encontrou_origem)
            {
                if(!(enderecos = realloc(enderecos,(qnt_enderecos + 1) * sizeof(endereco))))
                {
                    printf("Erro de alocação de memoria\n");
                    break;
                }
                
                /* Adiciona um novo endereco ao mapa */
                enderecos[qnt_enderecos].origem = strdup(end_origem);
                enderecos[qnt_enderecos].contador = 0;
                qnt_enderecos++;
            
            }
        linha++;
            
        } while(!feof(arff));

        /* Percorre o mapa e escreva as classificações no arquivo de saída */
        for (int i = 0; i < qnt_enderecos-1; i++)
        {
            if (enderecos[i].contador == 0)
                strcpy(classificacao, "benigna");
            else if (enderecos[i].contador <= 4) 
                strcpy(classificacao, "potencialmente maliciosa");
            else if(enderecos[i].contador > 4)
                strcpy(classificacao, "maliciosa");

            fprintf(arquivo, "%s;%s\n", enderecos[i].origem, classificacao);
        
        }
    } else 
        printf("O arquivo ainda nao contem os dados da seção @data\n");

    for (int i = 0; i < qnt_enderecos; i++)
       free(enderecos[i].origem);

    free(enderecos);

    fclose(arquivo);
}

void relatorio_media_tamanho(FILE *arff, atributo *atributos, int quantidade){

    char buffer_atributos[TAM_ATRIBUTO], buffer_dados[TAM_DADOS], *token, *dados;
    int media, qntd_categorias = 5, fim_arquivo, media_geral, atributo_de_ataque;
    info_media *calculo_media;

    FILE *arquivo;
    arquivo = fopen("R_TAMANHO.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    calculo_media = malloc(sizeof(info_media)* qntd_categorias);

    /* Posiciona o ponteiro de leitura para antes do @data */
    do {
        memset(buffer_atributos, 0, sizeof(buffer_atributos));

        fscanf(arff,"%[^\n]",buffer_atributos);
        fgetc(arff);

        if(!buffer_atributos[0])
            continue;

        token = strtok(buffer_atributos, " ");

    } while(token && strcmp(token, "@data") != 0);

    /* Inicializa os contadores dos ataques */
    for(int k = 0; k < qntd_categorias; k++){
        calculo_media[k].soma = 0;
        calculo_media[k].quant = 0;
    }

    /* Descarta a primeira linha @data */
    fim_arquivo = fscanf(arff,"%[^\n]",buffer_dados);
    fgetc(arff);

    if(fim_arquivo != -1)
    {
        do{
            int i = 0, j;
            memset(buffer_dados, 0, sizeof(buffer_dados));

            fscanf(arff,"%[^\n]",buffer_dados);
            fgetc(arff);
        
            dados = strtok(buffer_dados, ","); 
            
            while(dados && i < quantidade)
            {
                if(strcmp(atributos[i].rotulo, "PKT_AVG_SIZE") == 0)
                    media = atoi(dados);
              
                /* Verifica se o rotulo indica uma ocorrencia de ataque */
                if(dados && strcmp(atributos[i].rotulo, "PKT_CLASS") == 0)
                {
                    atributo_de_ataque = i;
                    /* Soma a quantidade de ocorrencias do pacote especifico. O pacote [j+1] corresponde ao ataque[j]*/
                    j = 0;
                    while(j < qntd_categorias)
                    {
                        if(strcmp(dados, atributos[i].categorias[j]) == 0){
                            calculo_media[j].soma += media;
                            calculo_media[j].quant ++;
                        }
                        j++;
                    }
                }

                dados = strtok(NULL, ",");

                if (!buffer_dados[0])
                    continue;

                i++;
            } 

            /* Testa se a quantidade de atriutos e de dados sao compativeis */
            if(dados){
                fclose(arquivo);
                printf("A quantidade de dados eh maior que a quantidade de atributos\n");
                return;
            } else if(i < quantidade)
            {
                fclose(arquivo);
                printf("A quantidade de dados eh menor que a quantidade de atributos\n");
                return;
            }

        } while(!feof(arff));

        if(atributo_de_ataque == -1)
            printf("O atributo PKT_CLASS nao existe no arquivo\n");
        {
            for (int i = 0; i < qntd_categorias; i++)
            {
                if (calculo_media[i].quant != 0) 
                {
                    media_geral = calculo_media[i].soma / calculo_media[i].quant;
                    fprintf(arquivo,"%s;%d\n", atributos[atributo_de_ataque].categorias[i], media_geral);
                } else 
                    fprintf(arquivo,"%s;0\n", atributos[atributo_de_ataque].categorias[i]);
            }
        }

    } else
        printf("O arquivo ainda nao contem os dados da seção @data\n");
    
    free(calculo_media);
    fclose(arquivo);
}

void lista_negra(FILE *arff, atributo *atributos)
{
    char buffer[1025];
    char *origem, *classificacao;
    FILE *input, *output;
    input = fopen("R_ENTIDADES.txt", "r");
    output = fopen("BLACKLIST.bl", "w");

    if (input == NULL) {
        printf("Erro ao abrir o arquivo R_ENTIDADES.txt\n");
        return;
    } else if(output == NULL){
        printf("Erro ao abrir o arquivo BLACKLIST.bl\n");
        return;
    }

    do{
        fscanf(input,"%[^\n]",buffer);
        fgetc(input);
        
        
        origem = strtok(buffer,";");
        classificacao = strtok(NULL, ";");

        if(classificacao && strcmp(classificacao,"maliciosa") == 0)
            fprintf(output,"%s\n",origem);

    }while(!feof(input));

    fclose(input);
    fclose(output);
}


