#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_PALAVRAS 400000
#define MAX_TAMANHO 31

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

    qsort(dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings);
}

int palavra_no_dicionario(const char *palavra_tokeada) {
    char palavra_lower[MAX_TAMANHO];
    strncpy(palavra_lower, palavra_tokeada, MAX_TAMANHO - 1);
    palavra_lower[MAX_TAMANHO - 1] = '\0';
    for (int i = 0; palavra_lower[i]; i++) {
        palavra_lower[i] = tolower(palavra_lower[i]);
    }

    return bsearch(&palavra_lower, dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings) != NULL;
}

void verificar_ortografia() {
    char texto[1000];

    while (1) {
        printf("Digite um texto (ou Ctrl+D para sair): ");
        if (fgets(texto, sizeof(texto), stdin) == NULL) {
            printf("\nNenhum texto identificado ou entrada encerrada.\n");
            break;
        }

        char *token;
        char *saveptr;
        token = strtok_r(texto, " ,.!?;:\"\n\t", &saveptr);
        while (token) {
            if (!palavra_no_dicionario(token)) {
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok_r(NULL, " ,.!?;:\"\n\t", &saveptr);
        }
    }
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
