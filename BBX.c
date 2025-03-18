#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_TAMANHO 31
#define CAPACIDADE_INICIAL 1000 // Initial capacity for the dictionary

// Structure to hold the dictionary
typedef struct {
    char **palavras;
    int num_palavras;
    int capacidade;
} Dicionario;

// Function to create a new dictionary
Dicionario* criar_dicionario(int capacidade) {
    Dicionario *dicionario = malloc(sizeof(Dicionario));
    dicionario->palavras = malloc(capacidade * sizeof(char *));
    dicionario->num_palavras = 0;
    dicionario->capacidade = capacidade;
    return dicionario;
}

// Function to free the dictionary
void liberar_dicionario(Dicionario *dicionario) {
    for (int i = 0; i < dicionario->num_palavras; i++) {
        free(dicionario->palavras[i]);
    }
    free(dicionario->palavras);
    free(dicionario);
}

// Function to compare strings (case-insensitive)
int comparar_strings(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Function to load the dictionary from a file
void carregar_dicionario(Dicionario *dicionario, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo do dicionário");
        exit(EXIT_FAILURE);
    }

    char palavra[MAX_TAMANHO];
    while (dicionario->num_palavras < dicionario->capacidade && fscanf(arquivo, "%30s", palavra) == 1) {
        // Allocate memory for the word and convert to lowercase
        dicionario->palavras[dicionario->num_palavras] = malloc(MAX_TAMANHO * sizeof(char));
        for (int i = 0; palavra[i]; i++) {
            dicionario->palavras[dicionario->num_palavras][i] = tolower(palavra[i]);
        }
        dicionario->palavras[dicionario->num_palavras][strlen(palavra)] = '\0'; // Null-terminate
        dicionario->num_palavras++;
    }

    fclose(arquivo);

    // Sort the dictionary
    qsort(dicionario->palavras, dicionario->num_palavras, sizeof(char *), comparar_strings);

    // Debug: Print the first 10 words to verify loading
    printf("Loaded %d words. First 10 words:\n", dicionario->num_palavras);
    for (int i = 0; i < 10 && i < dicionario->num_palavras; i++) {
        printf("%s\n", dicionario->palavras[i]);
    }
}

// Function to check if a word is in the dictionary
int palavra_no_dicionario(Dicionario *dicionario, const char *palavra_tokeada) {
    char palavra_lower[MAX_TAMANHO];
    strncpy(palavra_lower, palavra_tokeada, MAX_TAMANHO - 1);
    palavra_lower[MAX_TAMANHO - 1] = '\0';
    for (int i = 0; palavra_lower[i]; i++) {
        palavra_lower[i] = tolower(palavra_lower[i]);
    }

    // Debug: Print the word being searched
    printf("Searching for: %s\n", palavra_lower);

    // Use binary search to find the word
    return bsearch(&palavra_lower, dicionario->palavras, dicionario->num_palavras, sizeof(char *), comparar_strings) != NULL;
}

// Function to check spelling in user input
void verificar_ortografia(Dicionario *dicionario) {
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

            if (!palavra_no_dicionario(dicionario, token)) {
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " ,.!?;:\"\n\t");
        }
    }
}

int main(int argc, char *argv[]) {
    setlocale