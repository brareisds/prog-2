#include <stdio.h>
#include <stdlib.h>
#define TAM 5

typedef struct {
    int posicao;
    char ch;
} registro_t;

int main()
{
    char *poema;
    registro_t registro;
    int tamanho_arquivo, numero_registros;

    FILE *arquivo;
    arquivo = fopen("poema.bin", "rb");

    if (arquivo == NULL) 
    {
	    printf("Erro ao abrir o arquivo.\n");
	    return 1;
    }

    fseek(arquivo, 0, SEEK_END);
    tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    numero_registros = tamanho_arquivo / TAM;
  
    if (!(poema = malloc((numero_registros + 1) * sizeof(char)))) 
    {
        printf("Erro de alocação de memória!\n");
        fclose(arquivo);
        return 1;
    }

    while(fread(&registro, TAM, 1, arquivo))
        poema[registro.posicao] = registro.ch;
    
    poema[numero_registros] = '\0';
   
    printf("Poema: %s\n", poema);
    printf("\n");

    fclose(arquivo);
    
    free(poema);

    return 0;
}
