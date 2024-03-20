#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arff.h"
#define TAM_ATRIBUTO 1025
#define TAM_DADOS 2049

void exibe_atributos(atributo *infos, int quantidade) {
    int j;

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for (int i = 0; i < quantidade; i++) {
        printf("-> Atributo #%d\n", i + 1);
        printf("Rótulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) {
            j = 0;
            printf("Categorias: ");
            while (infos[i].categorias[j]) {
                printf("%s", infos[i].categorias[j]);
                j++;
                if (infos[i].categorias[j]) {
                    // Se não for a última categoria, imprime uma vírgula e espaço
                    printf(", ");
                }
            }
            printf("\n");
        }
        if (i < quantidade - 1) printf("------------------------------\n");
    }
    printf("===============================\n");
}


int conta_atributos(FILE *arff)
{

  char buffer[TAM_ATRIBUTO];
  int qntd = 0, fim_arquivo;
  char *token, *rotulo, *tipo, *teste;

  do
  {
    
    memset(buffer, 0, sizeof(buffer));  
    fim_arquivo = fscanf(arff,"%[^\n]",buffer);
    fgetc(arff);

    if(fim_arquivo == -1)
      return 0;

    if (!buffer[0])
      continue;

    token = strtok(buffer, " ");
    rotulo = strtok(NULL, " ");
    tipo = strtok(NULL, " ");

    teste = strtok(NULL, " ");
  
   /* Testa se há mais elementos que o necessario */
    if(teste)
      return 0;
   
    /* Verifica se a linha eh iniciada com o token correto e se o @data existe no arquivo*/ 
    if(token && strcmp(token, "@attribute") && strcmp(token, "@data"))
      return 0;

    
    /* Verifica se há menos elementos que o necessario */
    if(token && (!rotulo || !tipo) && strcmp(token, "@data"))
      return 0;
    

    /* Soma as linhas com token diferente de @data */
    if(strcmp(token, "@data") != 0)
      qntd++;

  } while(!token || strcmp(token, "@data") != 0);

  return qntd;

}

int conta_categorias(char *categorias)
{
  int qntd_categorias = 0;
  char *categoria, *temp;

  /* Cria uma cópia das categorias para não modificar a string original */
  temp = strdup(categorias);
  categoria = strtok(temp, ",");

  do {

    qntd_categorias++;
    categoria = strtok(NULL, ",");

  } while(categoria);

  free(temp);
  return qntd_categorias;
}

void processa_categorias(atributo *elemento, char *categorias)
{
  int qntd_categorias, i;
  char *categoria;
  
  qntd_categorias = conta_categorias(categorias);
  
  if(!(elemento->categorias = malloc((qntd_categorias + 1) * sizeof(char *)))){
    printf("Falha na alocação de memoria\n");
    return;
  }

  categoria = strtok(categorias, ",");

  i = 0;
  /* Preenche o vetor de categorias */
  while ( i < qntd_categorias) 
  {
    elemento->categorias[i] = strdup(categoria); 
    categoria = strtok(NULL, ",");
    i++;
  }

  /* Adiciona o caractere nulo ao final da string de categorias */
  elemento->categorias[i] = NULL;
}
    

atributo* processa_atributos(FILE *arff, int quantidade)
{
  char buffer[TAM_ATRIBUTO],*token = "", *rotulo = "", *tipo = "", *categorias = NULL;
  atributo *atributos; 
  int i = 0;

  if(!(atributos = malloc(sizeof(atributo) * quantidade)))
  {
    printf("Erro de alocação de memoria\n");
    return NULL;
  }
  
  do
  {
    /* Prepara o buffer para receber novas infos */
    memset(buffer, 0, sizeof(buffer));

    fscanf(arff,"%[^\n]",buffer);
    fgetc(arff);

    token = strtok(buffer, " ");
    rotulo = strtok(NULL, " ");
    tipo = strtok(NULL, " ");
    
    if(rotulo && tipo)
    {
      atributos[i].rotulo = strdup(rotulo);
      atributos[i].tipo = strdup(tipo);
    }

    /* Verifica se a linha esta em branco. */
    if (!buffer[0])
      continue;
    
    if(atributos[i].tipo)
    {
        if((!strcmp(atributos[i].tipo, "numeric"))|| (!strcmp(atributos[i].tipo, "string")))
          atributos[i].categorias = NULL;
        else
        {
          /* Remove as chaves do inicio e do fim da string antes de processa-la */
          categorias = malloc(strlen(atributos[i].tipo) - 1);
          strncpy(categorias, atributos[i].tipo + 1, strlen(atributos[i].tipo) - 2);
          categorias[strlen(atributos[i].tipo) - 2] = '\0'; 

          processa_categorias(&atributos[i], categorias);

          /* Libera a memória anterior, se alocada */
          free(atributos[i].tipo); 
          atributos[i].tipo = strdup("categoric"); 

        }
    }

    i++;

    free(categorias);
    categorias = NULL;
  
  } while((!token || strcmp(token, "@data") != 0) && i < quantidade);
  
  return atributos;
}

int eh_numerico(const char *dado)
{
    char *endptr;
    strtod(dado, &endptr);

    /* Se o ponteiro de término for o fim da string, é numérico. */
    if(*endptr == '\0')
      return 1;
    else 
      return 0; 

}


void valida_arff(FILE *arff, atributo *atributos, int quantidade)
{
  char buffer_atributos[TAM_ATRIBUTO], buffer_dados[TAM_DADOS], *token, *dados, *rotulo, *tipo, *teste;
  int valido = 1, encontrou_categoria, k, fim_arquivo;

  /* Posiciona o ponteiro de leitura para antes do @data */
  do {
    memset(buffer_atributos, 0, sizeof(buffer_atributos));

    fim_arquivo = fscanf(arff,"%[^\n]",buffer_atributos);

    if(fim_arquivo == -1)
      valido = 0;
  
    fgetc(arff);

    if (!buffer_atributos[0])
      continue;

    token = strtok(buffer_atributos, " ");
    rotulo = strtok(NULL, " ");
    tipo = strtok(NULL, " ");

    teste = strtok(NULL, " ");
  
   /*Testa se há mais elementos que o necessario */
    if(teste)
      valido = 0;

    /* Verifica se a linha eh iniciada com o token correto e se o @data existe no arquivo*/ 
    if(token && strcmp(token, "@attribute") && strcmp(token, "@data"))
      valido = 0;
    
    
    /* Verifica se há menos elementos que o necessario */
    if(token && (!rotulo || !tipo) && strcmp(token, "@data"))
      valido = 0;

  } while(token && strcmp(token, "@data") != 0);

  /* Descarta a primeira linha @data e verifica se o arquivo chegou ao fim */
  fim_arquivo = fscanf(arff,"%[^\n]",buffer_dados);
  fgetc(arff);

  if(fim_arquivo != -1)
  {
    /* Verifica se os dados sao compativeis */
    do{
      int i = 0;

      /* Inicializa um ponteiro para o token */
      dados = strtok(buffer_dados, ",");
       
      while(dados && valido && i < quantidade)
      {
        if(dados && strcmp(atributos[i].tipo, "numeric") == 0)
        {
          if(eh_numerico(dados))
            valido = 1;
          else
            valido = 0;
        }

        /* Verifica se o atributo eh do tipo categorico e em seguida percorre o vetor de categorias tentando encontrar o dado. */
        else if (dados && strcmp(atributos[i].tipo, "categoric") == 0)
        {
          encontrou_categoria = 0;
          k = 0;
          while(atributos[i].categorias[k] && !encontrou_categoria) 
          {
            if (strcmp(dados, atributos[i].categorias[k]) == 0)
              encontrou_categoria = 1;
            k++;
          }

          if(!encontrou_categoria)
            valido = 0;
          else
            valido = 1;
        }
          
        dados = strtok(NULL, ",");

        if (!buffer_dados[0])
          continue;

        i++;

      } 
        
      memset(buffer_dados, 0, sizeof(buffer_dados));

      fscanf(arff,"%[^\n]",buffer_dados);
      getc(arff);

    } while(!feof(arff));
  }

  if(!valido)
    printf("O arquivo arff eh invalido!\n");
  else 
    printf("O arquivo  arff eh valido!\n");

}

void destroi_atributos(atributo *atributos, int quantidade) 
{
    for (int i = 0; i < quantidade; i++)
    {
      free(atributos[i].rotulo);
      free(atributos[i].tipo);
      if (atributos[i].categorias) 
      {
        for (int j = 0; atributos[i].categorias[j]; j++)
            free(atributos[i].categorias[j]);
        free(atributos[i].categorias);
      }
    }
    free(atributos);
}
