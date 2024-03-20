#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfila.h"

void le_noticia(char *titulo, char *texto)
{
    getchar();
    printf("\nDigite o título: ");
    fgets(titulo, 33, stdin);
    printf("Digite o texto: ");
    fgets(texto, 513, stdin);
}


void imprime_fila(fila_t *fila)
{
    noticia_t *aux;
    aux = fila->ini;
  
    puts("=======================================================");
    while(aux)
    {
        printf("%s", aux->titulo);
        printf("%s", aux->texto);
    
        aux = aux->prox;
        if(aux)
            puts("==");
    }    
    puts("=======================================================");

}


void cadastra_noticia(fila_t *f)
{
    noticia_t *nova_noticia;

    /* Cria uma noticia, preenche com o titulo/texto e insere na fila do seu tipo especifico. */
    nova_noticia = cria_noticia();
    le_noticia(nova_noticia->titulo, nova_noticia->texto);
    enqueue(f,nova_noticia);
    
}


void fechamento_edicao(fila_t *breaking, fila_t *informes)
{
    fila_t *edicao;
    noticia_t *noticia_tipo0, *noticia_tipo1, *new_breaking, *new_informe;
    int noticias_validas;

    edicao = cria_fila();

    noticia_tipo0 = breaking->ini;
    noticia_tipo1 = informes->ini;
    noticias_validas = 0;

    /* Percorre primeiro a fila de breaking_news descartando as noticias invalidas.
     * Controla a quantidade e faz uma copia da noticia valida para a fila de edicao. */
    while(noticia_tipo0 && noticias_validas < 2)
    {
        if(noticia_tipo0->idade <= 3)
        {
            noticias_validas++;
            new_breaking = cria_noticia();

            strncpy(new_breaking->titulo, noticia_tipo0->titulo, (strlen(noticia_tipo0->titulo) + 1));
            strncpy(new_breaking->texto, noticia_tipo0->texto, (strlen(noticia_tipo0->texto) + 1));

            enqueue(edicao,new_breaking);
            dequeue(breaking);
        }
        else
            dequeue(breaking);
            
        noticia_tipo0 = breaking->ini;
        

    }

    /* Percorre por ultimo a fila de informes, caso a quantidade max nao foi atingida, descartando as noticias invalidas.
    * Faz uma copia da noticia valida para a fila de edicao. */
    while(noticia_tipo1 && noticias_validas < 2)
    {
        if(noticia_tipo1->idade <= 3)
        {
            noticias_validas++;
            new_informe = cria_noticia();

            strncpy(new_informe->titulo, noticia_tipo1->titulo, (strlen(noticia_tipo1->titulo) + 1));
            strncpy(new_informe->texto, noticia_tipo1->texto, (strlen(noticia_tipo1->texto) + 1));

            enqueue(edicao,new_informe);
            dequeue(informes);
            
        }
        else
            dequeue(informes);


        noticia_tipo1 = informes->ini;
    }

    /* Caso exista alguma noticia valida, a fila de edicao é impressa contendo apenas 
     * as noticias validas inseridas na ordem de prioridade. */
    if(noticias_validas == 0)
        printf("Esta edição foi pulada por falta de notícias!\n");
    else
        imprime_fila(edicao);
    
    fila_destroi(&edicao);
}


void incrementa_idade(fila_t *breaking, fila_t *informes)
{
    noticia_t *aux_breaking, *aux_informes;

    aux_breaking = breaking->ini;
    aux_informes = informes->ini;

    while(aux_breaking)
    {
        aux_breaking->idade++;
        aux_breaking = aux_breaking->prox;
    }

    while(aux_informes)
    {
        aux_informes->idade++;
        aux_informes = aux_informes->prox;
    }
}


int main()
{
    fila_t *breaking, *informes;
    int opcao, tipo;

    breaking = cria_fila();
    informes = cria_fila();

    do
    {
        printf("\n (1) Cadastrar notícia\n");
        printf(" (2) Fechar edição\n");
        printf(" (3) Sair\n");

        
        scanf(" %d",&opcao);

        if(opcao == 1)
        {
            printf("\n Digite o tipo da noticia:\n");
            printf(" (0) breaking_news\n");
            printf(" (1) informes\n");

            
            scanf(" %d",&tipo);

            if(tipo == 0)
                cadastra_noticia(breaking);
            else if(tipo == 1)
                cadastra_noticia(informes);
            else
                printf("Esse tipo nao eh valido\n");
        }

        if (opcao == 2)
        {
            fechamento_edicao(breaking,informes);
            incrementa_idade(breaking,informes);
        }

    } while(opcao != 3);

    fila_destroi(&breaking);
    fila_destroi(&informes);

    return 0;
}