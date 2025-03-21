#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//+++++++++++++++++++++++++++++++++++++++++++++ NAO UTILIZANDO ALOCAÇÃO DINAMICA++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_PALAVRAS 400000 // Número máximo de palavras
#define MAX_TAMANHO 31      // Tamanho máximo de cada palavra

// variáveis globais:
char dicionario[MAX_PALAVRAS][MAX_TAMANHO]; // Vetor estático para armazenar as palavras
int num_palavras = 0;                       // Contador de palavras carregadas

int comparar_strings(const void *a, const void *b)
{
    return strcasecmp((const char *)a, (const char *)b); // compara duas strings e retorna a hierarquia correta para a ordem alfabética
}

void carregar_dicionario(const char *nome_arquivo) //  leitura e organização do dicionario
{
    FILE *arquivo = fopen(nome_arquivo, "r"); // chama o arquivo para a leitura
    if (!arquivo)                             // se o arquivo der certo o numero vai ser diferente de 0 portanto nao executa o if
    {
        printf("Erro ao abrir o arquivo do dicionário!\n");
        exit(1); // termina o programa com status de erro
    }

    while (num_palavras < MAX_PALAVRAS && fscanf(arquivo, "%30s", dicionario[num_palavras]) == 1) // fscanf para pegar apenas uma palavra de no max 30 char tirando o \0
    {
        num_palavras++; // Enquanto há leitura de palavras pelo fscanf ele retorna o numero de elementos que ele leu. nesse caso 1. 0 quando nao ler e termina o ciclo
    }

    fclose(arquivo);

    qsort(dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings); // função que compara as strings
}

int palavra_no_dicionario(const char *palavra_tokeada) // pesquisa a palavra no vetor dicionario (busca binaria)
{
    return bsearch(palavra_tokeada, dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings) != NULL;
}

void verificar_ortografia() // compara a palavra lida no stdin com palavras no dicionario, retorna palavras erradas
{
    char texto[200];

    while (1) // Loop infinito até encontrar EOF
    {
        printf("Digite um texto (ou Ctrl+D para sair): ");

        if (fgets(texto, sizeof(texto), stdin) == NULL) // Verifica erro ou EOF
        { 
            printf("\nNenhum texto identificado ou entrada encerrada.\n");
            break;
        }
        texto[strcspn(texto, "\n")] = 0;
        char *token = strtok(texto, " ,.!?;:\"\n");

        while (token)
        {
            if (!palavra_no_dicionario(token)) // pesquisa a palavra no dicionário
            {
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " ,.!?;:\"\n");
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Uso: %s <arquivo_dicionario>\n", argv[0]);
        return 1;
    }

    carregar_dicionario(argv[1]); // função que recebe o dicionario

    printf("Escreva o texto a seguir: ");

    verificar_ortografia(); // função que identifica palavras erradas e mostra

    return 0;
}