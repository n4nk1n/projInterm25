#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>

#define MAX_PALAVRAS 400000
#define MAX_TAMANHO 31

char dicionario[MAX_PALAVRAS][MAX_TAMANHO];
int num_palavras = 0;

// Função para comparar strings (case-sensitive, pois o dicionário já está em minúsculas)
int comparar_strings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

// Função para carregar o dicionário
void carregar_dicionario(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo do dicionário");
        exit(1);
    }

    while (num_palavras < MAX_PALAVRAS && fscanf(arquivo, "%30s", dicionario[num_palavras]) == 1) {
        for (int i = 0; dicionario[num_palavras][i]; i++) {
            dicionario[num_palavras][i] = tolower((unsigned char)dicionario[num_palavras][i]);
        }
        num_palavras++;
    }

    fclose(arquivo);

    // Debug: Print the first 10 words to verify loading
    printf("Carregadas %d palavras. Primeiras 10 palavras:\n", num_palavras);
    for (int i = 0; i < 10 && i < num_palavras; i++) {
        printf("%s\n", dicionario[i]);
    }

    qsort(dicionario, num_palavras, MAX_TAMANHO, comparar_strings);
}

// Função para verificar se uma palavra está no dicionário
int palavra_no_dicionario(const char *palavra_tokeada) {
    if (strlen(palavra_tokeada) >= MAX_TAMANHO) {
        printf("Palavra muito longa: %s\n", palavra_tokeada);
        return 0;
    }

    char palavra_lower[MAX_TAMANHO];
    strncpy(palavra_lower, palavra_tokeada, MAX_TAMANHO - 1); // Copy up to MAX_TAMANHO - 1 characters
    palavra_lower[MAX_TAMANHO - 1] = '\0'; // Ensure null-termination

    for (int i = 0; palavra_lower[i]; i++) {
        palavra_lower[i] = tolower((unsigned char)palavra_lower[i]);
    }

    // Debug: Print the word being searched
    printf("Procurando por: %s\n", palavra_lower);

    return bsearch(palavra_lower, dicionario, num_palavras, MAX_TAMANHO, comparar_strings) != NULL;
}

// Função para verificar ortografia de um texto
void verificar_ortografia() {
    char texto[1000];

    while (1) {
        printf("Digite um texto (ou Ctrl+D para sair): ");
        if (fgets(texto, sizeof(texto), stdin) == NULL) {
            printf("\nNenhum texto identificado ou entrada encerrada.\n");
            break;
        }

        char *token = strtok(texto, " ,.!?;:\"\n\t");
        while (token) {
            // Debug: Print each token
            printf("Token: %s\n", token);

            if (!palavra_no_dicionario(token)) {
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " ,.!?;:\"\n\t");
        }
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8"); // Configuração de locale para UTF-8
    if (argc != 2) {
        printf("Uso: %s <arquivo_dicionario>\n", argv[0]);
        return 1;
    }

    carregar_dicionario(argv[1]);
    verificar_ortografia();

    return 0;
}
