#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_TAMANHO 31

char **dicionario;
int num_palavras = 0;

int comparar_strings(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

void carregar_dicionario(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo do dicionário");
        exit(1);
    }

    char palavra[MAX_TAMANHO];
    while (fscanf(arquivo, "%30s", palavra) == 1) {
        dicionario = realloc(dicionario, (num_palavras + 1) * sizeof(char *));
        if (!dicionario) {
            perror("Erro ao realocar memória");
            exit(1);
        }
        dicionario[num_palavras] = malloc(MAX_TAMANHO * sizeof(char));
        if (!dicionario[num_palavras]) {
            perror("Erro ao alocar memória para palavra");
            exit(1);
        }
        for (int i = 0; palavra[i]; i++) {
            palavra[i] = tolower(palavra[i]);
        }
        // warning: ‘__builtin_strncpy’ output may be truncated copying 30 bytes
        //from a string of length 30 [-Wstringop-truncation]
        strncpy(dicionario[num_palavras], palavra, MAX_TAMANHO - 1); 
        dicionario[num_palavras][MAX_TAMANHO - 1] = '\0';
        num_palavras++;
    }

    fclose(arquivo);

    // Debug: Print the first 10 words to verify loading
    printf("Loaded %d words. First 10 words:\n", num_palavras);
    for (int i = 0; i < 10 && i < num_palavras; i++) {
        printf("%s\n", dicionario[i]);
    }

    qsort(dicionario, num_palavras, sizeof(char *), comparar_strings);
}

int palavra_no_dicionario(const char *palavra_tokeada) {
    char palavra_lower[MAX_TAMANHO];
    strncpy(palavra_lower, palavra_tokeada, MAX_TAMANHO - 1); // Copy up to MAX_TAMANHO - 1 characters
    palavra_lower[MAX_TAMANHO - 1] = '\0'; // Ensure null-termination
    for (int i = 0; palavra_lower[i]; i++) {
        palavra_lower[i] = tolower(palavra_lower[i]);
    }

    // Debug: Print the word being searched
    printf("Searching for: %s\n", palavra_lower);

    return bsearch(&palavra_lower, dicionario, num_palavras, sizeof(char *), comparar_strings) != NULL;
}

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
    setlocale(LC_ALL, "en_US.UTF-8");
    if (argc != 2) {
        printf("Uso: %s <arquivo_dicionario>\n", argv[0]);
        return 1;
    }

    dicionario = NULL;
    carregar_dicionario(argv[1]);
    verificar_ortografia();

    // Free allocated memory
    for (int i = 0; i < num_palavras; i++) {
        free(dicionario[i]);
    }
    free(dicionario);

    return 0;
}