#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>

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
        // Convert to lowercase
        for (int i = 0; dicionario[num_palavras][i]; i++) {
            dicionario[num_palavras][i] = tolower(dicionario[num_palavras][i]);
        }
        num_palavras++;
    }

    fclose(arquivo);
    
    // Sort the dictionary for binary search
    qsort(dicionario, num_palavras, MAX_TAMANHO, comparar_strings);
    
    printf("Dicionário carregado com %d palavras.\n", num_palavras);
}

bool palavra_no_dicionario(const char *palavra) {
    if (!palavra || strlen(palavra) == 0) {
        return true; // Empty strings are considered valid
    }
    
    // Create a lowercase copy of the word
    char palavra_lower[MAX_TAMANHO];
    size_t len = strlen(palavra);
    if (len >= MAX_TAMANHO) {
        len = MAX_TAMANHO - 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        palavra_lower[i] = tolower(palavra[i]);
    }
    palavra_lower[len] = '\0';
    
    // Perform binary search
    return bsearch(palavra_lower, dicionario, num_palavras, MAX_TAMANHO, comparar_strings) != NULL;
}

void verificar_ortografia() {
    char texto[MAX_TEXTO];
    char *token;
    
    printf("\n=== Verificador de Ortografia ===\n");
    printf("Digite um texto (ou Ctrl+D para sair):\n");
    
    while (fgets(texto, sizeof(texto), stdin)) {
        // Process each word in the input
        token = strtok(texto, " ,.!?;:\"\n\t");
        bool erros_encontrados = false;
        
        while (token) {
            if (!palavra_no_dicionario(token)) {
                if (!erros_encontrados) {
                    printf("\nPalavras não encontradas no dicionário:\n");
                    erros_encontrados = true;
                }
                printf("- \"%s\"\n", token);
            }
            token = strtok(NULL, " ,.!?;:\"\n\t");
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