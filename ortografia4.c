#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>

#define MAX_PALAVRAS 400000
#define MAX_TAMANHO 31
#define MAX_TEXTO 1000

char dicionario[MAX_PALAVRAS][MAX_TAMANHO];
int num_palavras = 0;

int comparar_strings(const void *a, const void *b) {
    return strcasecmp((const char *)a, (const char *)b);
}

void carregar_dicionario(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo do dicionário");
        exit(1);
    }

    while (num_palavras < MAX_PALAVRAS && fscanf(arquivo, "%30s", dicionario[num_palavras]) == 1) {
        for (int i = 0; dicionario[num_palavras][i]; i++) {
            dicionario[num_palavras][i] = tolower(dicionario[num_palavras][i]);
        }
        num_palavras++;
    }

    fclose(arquivo);
    qsort(dicionario, num_palavras, MAX_TAMANHO, comparar_strings);
    printf("Dicionário carregado com %d palavras.\n", num_palavras);
}

bool palavra_no_dicionario(const char *palavra) {
    if (!palavra || strlen(palavra) == 0) {
        return true;
    }
    return bsearch(palavra, dicionario, num_palavras, MAX_TAMANHO, comparar_strings) != NULL;
}

void verificar_ortografia() {
    char texto[MAX_TEXTO];
    char *palavra;

    printf("\n=== Verificador de Ortografia ===\n");
    printf("Digite um texto (ou Ctrl+D para sair):\n");

    while (fgets(texto, sizeof(texto), stdin)) {
        palavra = strtok(texto, " ,.!?;:\"\n\t");
        bool erros_encontrados = false;

        while (palavra) {
            char palavra_lower[MAX_TAMANHO];
            strncpy(palavra_lower, palavra, MAX_TAMANHO - 1);
            palavra_lower[MAX_TAMANHO - 1] = '\0';
            for (int i = 0; palavra_lower[i]; i++) {
                palavra_lower[i] = tolower(palavra_lower[i]);
            }

            if (!palavra_no_dicionario(palavra_lower)) {
                if (!erros_encontrados) {
                    printf("\nPalavras não encontradas no dicionário:\n");
                    erros_encontrados = true;
                }
                printf("- \"%s\"\n", palavra);
            }
            palavra = strtok(NULL, " ,.!?;:\"\n\t");
        }

        if (!erros_encontrados) {
            printf("\nNenhum erro de ortografia encontrado.\n");
        }

        printf("\nDigite outro texto (ou Ctrl+D para sair):\n");
    }

    printf("\nVerificação de ortografia finalizada.\n");
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8");
    if (argc != 2) {
        printf("Uso: %s <arquivo_dicionario>\n", argv[0]);
        return 1;
    }

    carregar_dicionario(argv[1]);
    verificar_ortografia();
    return 0;
}
