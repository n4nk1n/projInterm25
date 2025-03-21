#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMO 141

// Declarar funções
void carregar_arquivo(const char *arquivo);
int compara_string(const void *a, const void *b);
void escreve(const char *RunResults_out);

// Definição dos escalões
const char *escalao[] = {"F20", "F40", "F45", "M20", "M35", "M40", "M45", "M50", "M55", "M60"};
char *linhas[MAXIMO];
int num_linhas = 0; // Contador de linhas carregadas

// Função de comparação para qsort
int compara_string(const void *a, const void *b)
{
    char *linhaA = *(char **)a;
    char *linhaB = *(char **)b;

    char escalaoA[4], escalaoB[4];
    sscanf(linhaA, "%*d\t%s", escalaoA); 
    sscanf(linhaB, "%*d\t%s", escalaoB);

    int posA = -1, posB = -1;
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(escalaoA, escalao[i]) == 0) posA = i;
        if (strcmp(escalaoB, escalao[i]) == 0) posB = i;
    }

    if (posA == posB) {
        int posicaoA, posicaoB;
        sscanf(linhaA, "%d", &posicaoA); // Posição da linha A
        sscanf(linhaB, "%d", &posicaoB); // Posição da linha B
        return posicaoA - posicaoB; // Ordena pela posição geral
    }

    return posA - posB;
}

// Função para escrever o arquivo de saída
void escreve(const char *RunResults_out)
{
    FILE *arquivo = fopen(RunResults_out, "w");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo para escrita");
        exit(1);
    }

    for (int j = 0; j < num_linhas; j++) // Escrever apenas as linhas carregadas
    {
        fputs(linhas[j], arquivo);
        free(linhas[j]); // Liberar a memória alocada
    }

    fclose(arquivo);
}

// Função para carregar o arquivo e filtrar apenas os escalões femininos
void carregar_arquivo(const char *RunResults)
{
    FILE *arquivo = fopen(RunResults, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo");
        exit(1);
    }

    char buffer[256];

    fgets(buffer, sizeof(buffer), arquivo); // Ignorar cabeçalho

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL)
    {
        char escalao[4];
        sscanf(buffer, "%*d\t%s", escalao); // Extrai o escalão

        if (escalao[0] != 'F') continue; // Apenas escalões que começam com F

        int len = strlen(buffer);
        linhas[num_linhas] = (char *)malloc((len + 1) * sizeof(char));

        if (!linhas[num_linhas])
        {
            printf("Erro ao alocar memória");
            exit(1);
        }

        strcpy(linhas[num_linhas], buffer);
        num_linhas++; // Incrementa o número de linhas carregadas
    }

    qsort(linhas, num_linhas, sizeof(char *), compara_string);
    fclose(arquivo);
}

// ----------------------------------main----------------------------------------------------------
int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <arquivo_corredores> <arquivo_organizado>\n", argv[0]);
        return 1;
    }

    carregar_arquivo(argv[1]);
    escreve(argv[2]);

    return 0;
}