#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 
#include "tokens.h"
#include "lexico.c"


int main() {
    FILE *arquivo = fopen("teste.txt", "r");
    FILE *saida = fopen("saida.txt", "w");
    
    if (arquivo == NULL || saida == NULL) {
        perror("Erro ao abrir os arquivos");
        return 1;
    }

    // Chame a função de análise léxica
    analise_lexica(arquivo, saida);

    // Imprima a tabela de símbolos
    printSymbolTable();

    // Feche os arquivos
    fclose(arquivo);
    fclose(saida);

    return 0;
}