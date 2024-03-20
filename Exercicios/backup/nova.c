#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned tamanho; // soma do tamanho do nome e o tamanho do conteudo
   

} registro_t;

int main(int argc, char *argv[])
{
    /*if (argc != 2) {
        fprintf(stderr, "Uso incorreto. Use:\n");
        fprintf(stderr, "  ./backup\n");
        fprintf(stderr, "ou\n");
        fprintf(stderr, "  ./backup -t\n");
        return 1;
    }*/

    unsigned quant_registros, tam_registro,tam_str, tam_conteudo;;
    registro_t registro;
    char nome[100];
    int i;

    FILE *arquivo = fopen("bkp.bin", "rb");

    if(!arquivo)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    fread(&quant_registros,sizeof(unsigned),1,arquivo);
    printf("Quantidade registros: %u\n", quant_registros);

    int j = 0, tam_total =0;
    while(j < quant_registros)
    {
        //printf("registro: %d\n",j);
        tam_str = 0;
        fread(&tam_registro,sizeof(unsigned),1,arquivo);
        //printf("Tamanho do registro: %u\n",tam_registro);
        tam_total += tam_registro;
        fscanf(arquivo, "%[^\n]",nome);
        while(nome[tam_str] != '\0')
            tam_str++;
         
        fgetc(arquivo);
       // printf("tamanho string: %u\n",tam_str);
        //printf("nome do arquivo: %s\n",nome);
        
        tam_conteudo = tam_registro - (tam_str + 1);
        //printf("Tamanho do conteudo: %u\n", tam_conteudo);

        if (strcmp(argv[1], "-t") == 0) {
            printf("Nome do arquivo: %s, Tamanho: %u\n", nome, tam_registro);
        } else if (strcmp(argv[1], "-t") != 0 && strcmp(argv[1], "./backup") != 0) {
            fprintf(stderr, "Opção inválida.\n");
            fclose(arquivo);
            return 1;
        }

        fseek(arquivo,tam_conteudo,SEEK_CUR);
        j++;
    }

    if (strcmp(argv[1], "./backup") == 0) {
        // Extrair os arquivos armazenados em bkp.bin para o diretório corrente: ./backup
        char command[100];
        strcpy(command, "mkdir -p ./backup");
        system(command);

        fseek(arquivo, sizeof(unsigned), SEEK_SET);
        j = 0;
        while (j < quant_registros) {
            fread(&tam_registro, sizeof(unsigned), 1, arquivo);
            fscanf(arquivo, "%[^\n]", nome);

            FILE *output_file = fopen(nome, "wb");
            if (!output_file) {
                fprintf(stderr, "Erro ao criar o arquivo de saída.\n");
                fclose(arquivo);
                return 1;
            }

            char *conteudo = (char *)malloc(tam_registro - sizeof(unsigned) - strlen(nome) - 2);
            fread(conteudo, 1, tam_registro - sizeof(unsigned) - strlen(nome) - 2, arquivo);

            fwrite(conteudo, 1, tam_registro - sizeof(unsigned) - strlen(nome) - 2, output_file);

            fclose(output_file);
            free(conteudo);

            j++;
        }
    }

    fseek(arquivo, 0, SEEK_END);
    int tamanho_arquivo = ftell(arquivo);
    printf("Tamanho do arquivo: %d\n",tamanho_arquivo);
    printf("Tamanho total: %d\n",tam_total);
        
    fclose(arquivo);


    return 0;
}