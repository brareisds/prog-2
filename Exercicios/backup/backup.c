#include <stdio.h>

typedef struct {
    unsigned tamanho; // soma do tamanho do nome e o tamanho do conteudo
    //char *nome; //string terminada com \n
    // int conteudo? obg dnd kkk te amoeu que te amo mtBELEZAFECHOU ENTAOkkk; 

} registro_t;

int main()
{
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
        printf("registro: %d\n",j);
        tam_str = 0;
        fread(&tam_registro,sizeof(unsigned),1,arquivo);
        printf("Tamanho do registro: %u\n",tam_registro);
        tam_total += tam_registro;
        fscanf(arquivo, "%[^\n]",nome);
        while(nome[tam_str] != '\0')
            tam_str++;
         
        fgetc(arquivo);
        printf("tamanho string: %u\n",tam_str);
        printf("nome do arquivo: %s\n",nome);
        
        tam_conteudo = tam_registro - (tam_str + 1);
        printf("Tamanho do conteudo: %u\n", tam_conteudo);
        fseek(arquivo,tam_conteudo,SEEK_CUR);
        j++;
    }

    fseek(arquivo, 0, SEEK_END);
    int tamanho_arquivo = ftell(arquivo);
    printf("Tamanho do arquivo: %d\n",tamanho_arquivo);
        
    

    fclose(arquivo);


    return 0;
}