#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "log.h"


#define HELP "Forma de uso: ./arff -i <arq_in> [-p -v -a -b -c -d]\n"

int main(int argc, char **argv){
  int opt;
  char exibicao = 0;
  char validacao = 0;
  char ataques = 0;
  char entidades = 0;
  char tamanho = 0;
  char firewall = 0;
  char *entrada = 0;

  while ((opt = getopt(argc, argv, "pvabcdi:")) != -1) {
    switch (opt) {
    case 'i':
      entrada = strdup(optarg);
      break;
    case 'p':
      exibicao = 1;
      break;
    case 'v':
      validacao = 1;
      break;
    case 'a': //Relatório de todos os ataques ocorridos e o número de ocorrências no dataset (nome do arquivo de saída: R_ATAQUES.txt)
      ataques = 1;
      break;
    case 'b': //Relatório de endereços de origem maliciosos, pot. maliciosos e benignos (nome do arquivo de saída: R_ENTIDADES.txt)
      entidades = 1;
      break;
    case 'c': //A média da média do tamanho dos pacotes por cada tipo de ataque, considerando cada origem (nome do arquivo de saída: R_TAMANHO.txt)
      tamanho = 1; 
      break;
    case 'd': //Geração de um arquivo de configuração de iptable para bloquear as origens maliciosas (nome do arquivo de saída: C_IPTABLES.txt)
      firewall = 1;
      break;

    default:
      fprintf(stderr, HELP);
      exit(1);
    }
  }

  if (!entrada){
    fprintf(stderr, HELP);
    exit(2);
  }

  FILE *arquivo = fopen(entrada, "r");
  if (!arquivo){
    free(entrada);
    fprintf(stderr, "Não foi possível abrir o arquivo informado!\n");
    exit(3);
  }

  int qntd_atributos = conta_atributos(arquivo);
  
  if (!qntd_atributos){
    free(entrada);
    fclose(arquivo);
    fprintf(stderr, "Existem erros no arquivo fornecido!\n");
    exit(4);
  }
  
  rewind(arquivo);
  atributo *dados_atributos = processa_atributos(arquivo, qntd_atributos);

  if(!dados_atributos)
    printf("Falha de alocação de memoria\n");

  if (exibicao){
    exibe_atributos(dados_atributos, qntd_atributos);
  }
  if (validacao){
    rewind(arquivo);
    valida_arff(arquivo, dados_atributos, qntd_atributos);
  }
  if (ataques){
    rewind(arquivo);
    relatorio_ataques(arquivo, dados_atributos,qntd_atributos);
  }
  if (entidades){
    rewind(arquivo);
    relatorio_enderecos_maliciosos(arquivo, dados_atributos,qntd_atributos);
  }
  if (tamanho){
    rewind(arquivo);
    relatorio_media_tamanho(arquivo, dados_atributos,qntd_atributos);
  }
  if (firewall){
    rewind(arquivo);
    lista_negra(arquivo, dados_atributos);
  }


  fclose(arquivo);

  destroi_atributos(dados_atributos, qntd_atributos);
  free(entrada);

  return 0;
}
