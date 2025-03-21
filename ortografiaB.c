#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_PALAVRAS 400000  // Número máximo de palavras
#define MAX_TAMANHO 31       // Tamanho máximo de cada palavra
#define MAX_LINHA 512        // Evita truncamento

char dicionario[MAX_PALAVRAS][MAX_TAMANHO];
int num_palavras = 0;  // Contador de palavras carregadas

// Comparação para ordenação e busca binária
int comparar_strings(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Função que carrega o dicionário para a memória
void carregar_dicionario(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo do dicionário!\n");
        exit(1);
    }

    while (num_palavras < MAX_PALAVRAS && fscanf(arquivo, "%30s", dicionario[num_palavras]) == 1) {
        num_palavras++;
    }

    fclose(arquivo);

    // Ordenação para busca binária
    qsort(dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings);
}

// Pesquisa se a palavra existe no dicionário
int palavra_no_dicionario(const char *palavra_tokeada) {
    return bsearch(&palavra_tokeada, dicionario, num_palavras, sizeof(dicionario[0]), comparar_strings) != NULL;
}

// Função que verifica a ortografia do texto inserido
void verificar_ortografia() {
    char texto[MAX_LINHA];

    while (1) {
        printf("Digite um texto (ou Ctrl+D para sair): ");

        if (fgets(texto, sizeof(texto), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nEntrada encerrada (EOF detectado).\n");
            } else {
                printf("\nErro ao ler a entrada.\n");
            }
            break;
        }

        texto[strcspn(texto, "\n")] = 0;  // Remove '\n' do final da string

        // Tokeniza e verifica cada palavra
        char *token = strtok(texto, " ,.!?;:\"()\t\n");
        while (token) {
            // Remover plicas externas, mas manter internas
            size_t len = strlen(token);
            if (len > 1 && (token[0] == '\'' || token[len - 1] == '\'')) {
                token[len - 1] = '\0';
                token++;
            }

            if (!palavra_no_dicionario(token)) {
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " ,.!?;:\"()\t\n");
        }
    }
}

int main(int argc, char *argv[]) {


    if (argc != 2) {
        printf("Uso: %s <arquivo_dicionario>\n", argv[0]);
        return 1;
    }

    carregar_dicionario(argv[1]);
    verificar_ortografia();

    return 0;
}
