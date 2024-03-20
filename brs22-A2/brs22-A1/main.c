#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *rotulo;
  char *tipo;
  char *categorias;
} atributo;

void exibe_atributos(atributo *infos, int tamanho){
    if (infos == 0){
        printf("O arquivo ARFF fornecido � inv�lido!\n");
        exit(0);
    }

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for(int i = 0; i < tamanho; i++){
        printf("-> Atributo #%d\n", i+1);
        printf("Rótulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < tamanho-1) printf("------------------------------\n");
    }
    printf("===============================\n");
}

int conta_atributos(FILE *arff)
{
  char buffer[1024];
  int qntd = 0,teste;
  char *token;

  do
  {
    fscanf(arff,"%[^\n]",buffer);
    fgetc(arff);

    if (!buffer[0])
      continue;

    /* Recebe a primeira palavra da linha para verificar a condicao do loop */
    token = strtok(buffer, " ");

    /*Interrompe o while para evitar que entre em loop por conta da funcao memset */
    if(strcmp(token, "@data") == 0)
      break;

    memset(buffer, 0, sizeof(buffer));    

    qntd++;
  
  } while(!token || strcmp(token, "@data") != 0);

  return qntd;

}

atributo* processa_atributos(FILE *arff)
{

  char buffer[1024];
  atributo *atributos; 
  char *token, *rotulo, *tipo, *teste;
  int i = 0, qntd_atributos;

  qntd_atributos = conta_atributos(arff);
  rewind(arff);

  if(!(atributos = malloc(sizeof(atributo) * (qntd_atributos + 1))))
  {
    printf("Erro de alocação de memoria\n");
    return NULL;
  }

  do
  {
    fscanf(arff,"%[^\n]",buffer);
    fgetc(arff);

    token = strtok(buffer, " ");
    rotulo = strtok(NULL, " ");
    tipo = strtok(NULL, " ");

    teste = strtok(NULL, " ");

   /*Testa se há mais elementos que o necessario */
    if(teste)
      return NULL;
    
    if(rotulo && tipo)
    {
      atributos[i].rotulo = strdup(rotulo);
      atributos[i].tipo = strdup(tipo);
    }

    /* Verifica se a linha esta em branco. */
    if (!buffer[0])
      continue;

    /* Interrompe o while para a condicao nao entrar em loop */
    if(!strcmp(token, "@data"))
      break;

    /* Verifica se a linha eh iniciada com o token correto. */
    if(token && strcmp(token, "@attribute"))
      return NULL;

    /* Verifica se há menos elementos que o necessario */
    if(token && (!rotulo || !tipo))
        return NULL;
    
    if(atributos[i].tipo)
    {
        if((!strcmp(atributos[i].tipo, "numeric"))|| (!strcmp(atributos[i].tipo, "string")))
          atributos[i].categorias = NULL;
        else
        {
          atributos[i].categorias = atributos[i].tipo;
          atributos[i].tipo = "categoric";
        }
    }

    i++;
    
    /* Prepara o buffer para recever novas infos */
    memset(buffer, 0, sizeof(buffer));

    } while(!token || strcmp(token, "@data") != 0);

    rewind(arff);
    return atributos;
}

void libera_atributos(atributo *atributos, int tam)
{
  for(int i = 0; i < tam; i++)
  {
    if((!strcmp(atributos[i].tipo, "numeric"))|| (!strcmp(atributos[i].tipo, "string")))
      free(atributos[i].tipo);
    free(atributos[i].rotulo);
    if(atributos[i].categorias)
      free(atributos[i].categorias); 
  }
   free(atributos);
}


int main(int argc, char **argv)
{
  int opt, tam, teste;
  char exibicao = 0;
  char *entrada = 0;
  atributo *atributos;
  FILE *arff; 

   while ((opt = getopt(argc, argv, "pi:")) != -1) {
    switch (opt) {
    case 'i':
      entrada = strdup(optarg);
      break;
    case 'p':
      exibicao = 1;
      break;
    default:
      fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
      exit(1);
    }
  }

  if ( ! entrada ) {
    fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
    exit(2);
  }

  arff = fopen(entrada, "r");

  if (!arff){
    printf("Erro ao abrir o arquivo\n");
    return 1;
  }

  atributos = processa_atributos(arff);
  
  if(exibicao) 
  {
    tam = conta_atributos(arff);
    exibe_atributos(atributos, tam);
  }
  
  fclose(arff);

  libera_atributos(atributos,tam);
  free(entrada);
  
  return 0 ;
}