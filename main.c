#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definindo constantes para os tokens
#define KW_CHAR      1
#define KW_INT       2
#define KW_REAL      3
#define KW_BOOL      4
#define KW_IF        5
#define KW_THEN      6
#define KW_ELSE      7
#define KW_WHILE     8
#define KW_INPUT     9
#define KW_OUTPUT    10
#define KW_RETURN    11
#define TK_IDENTIFICADOR  12
#define INT_LITERAL  13
#define REAL_LITERAL 14
#define CHAR_LITERAL 15
#define STRING_LITERAL 16
#define OPERADOR_VIRGULA  17
#define OPERADOR_PONTOVIRGULA  18
#define OPERADOR_LPAREN  19
#define OPERADOR_RPAREN  20
#define OPERADOR_COLCHETE_ESQ 21
#define OPERADOR_COLCHETE_DIR 22
#define OPERADOR_CHAVE_ESQ  23
#define OPERADOR_CHAVE_DIR  24
#define OPERADOR_ATRIBUICAO  25
#define OPERADOR_SOMA  26
#define OPERADOR_SUBTRACAO 27
#define OPERADOR_MULTIPLICACAO 28
#define OPERADOR_DIVISAO  29
#define OPERADOR_MODULO  30
#define OPERADOR_MENOR 31
#define OPERADOR_MAIOR 32
#define OPERADOR_E  33
#define OPERADOR_OU   34
#define OPERADOR_NAO  35
#define OPERADOR_LE   36
#define OPERADOR_GE   37
#define OPERADOR_EQ   38
#define OPERADOR_DIF  39
#define OPERADOR_PONTO 40


// Função para verificar se uma palavra é uma palavra reservada
int eh_palavra_reservada(char *palavra) {
    char *palavras_reservadas[] = {
        "char", "int", "real", "bool", "if", "then", "else", "while", "input", "output", "return"
    };
    int num_palavras_reservadas = sizeof(palavras_reservadas) / sizeof(palavras_reservadas[0]);
    for (int i = 0; i < num_palavras_reservadas; i++) {
        if (strcmp(palavra, palavras_reservadas[i]) == 0) {
            return 1; // É uma palavra reservada
        }
    }
    return 0; // Não é uma palavra reservada
}

int obter_token_operador(char operador) {
    char operadores[] = {',', ';', '(', ')', '[', ']', '{', '}', '+', '-', '*', '/', '%', '&', '|', '~', '=', '<', '>'};
    int tokens[] = {OPERADOR_VIRGULA, OPERADOR_PONTOVIRGULA, OPERADOR_LPAREN, OPERADOR_RPAREN, OPERADOR_COLCHETE_ESQ, OPERADOR_COLCHETE_DIR, OPERADOR_CHAVE_ESQ, OPERADOR_CHAVE_DIR, OPERADOR_SOMA, OPERADOR_SUBTRACAO, OPERADOR_MULTIPLICACAO, OPERADOR_DIVISAO, OPERADOR_MODULO, OPERADOR_E, OPERADOR_OU, OPERADOR_NAO, OPERADOR_ATRIBUICAO, OPERADOR_MENOR, OPERADOR_MAIOR};
    
    int num_operadores = sizeof(operadores) / sizeof(operadores[0]);
    
    for (int i = 0; i < num_operadores; i++) {
        if (operador == operadores[i]) {
            return tokens[i];
        }
    }
    
    return -1; // Operador desconhecido
}


// Função para verificar operadores compostos
// Função para verificar operadores compostos
int verificar_operador_composto(FILE *arquivo, char *token) {
    int c = fgetc(arquivo);
    if (c == EOF) {
        return -1; // Fim do arquivo
    }
    
    switch (token[0]) {
        case '<':
        case '>':
        case '=':
        case '!':
        case '+':
            if (c == '=') {
                token[1] = c;
                token[2] = '\0';
                return obter_token_operador(token[0]);
            }
            ungetc(c, arquivo); // Coloca o caractere de volta no arquivo
            break;
    }
    
    return -1; // Não é um operador composto
}

int verificar_string(FILE *arquivo, char *token) {
    int c = fgetc(arquivo);
    if (c == '"') {
        int i = 0;
        token[i++] = '"';
        c = fgetc(arquivo);
        while (c != EOF && c != '"') {
            token[i++] = c;
            c = fgetc(arquivo);
        }
        if (c == '"') {
            token[i++] = '"';
            token[i] = '\0';
            return STRING_LITERAL;
        }
    }
    return -1; // Não é uma string
}

int verificar_pontuacao(FILE *arquivo, char *token) {
    if (token[0] == '.') {
        return OPERADOR_PONTO;
    } else if (token[0] == ';') {
        return OPERADOR_PONTOVIRGULA;
    }
    return -1; // Não é um ponto ou ponto e vírgula
}

// Função para analisar caracteres literais


// Função principal para análise léxica
// Função principal para análise léxica
void analise_lexica(FILE *arquivo) {
    char token[100];
    int c;
    
    while ((c = fgetc(arquivo)) != EOF) {
        if (isspace(c)) {
            // Ignorar espaços em branco
            continue;
        } else if (isalpha(c) || c == '_') {
            // Identificador
            int i = 0;
            while (isalnum(c) || c == '_') {
                token[i++] = c;
                c = fgetc(arquivo);
            }
            token[i] = '\0';
            if (eh_palavra_reservada(token)) {
                printf("Token: KW_%s\t\tLexema: %s\n", token, token);
            } else {
                printf("Token: TK_IDENTIFICADOR\tLexema: %s\n", token);
            }
            ungetc(c, arquivo); // Coloca o caractere de volta no arquivo
        } else if (isdigit(c)) {
            // Literal inteiro ou real
            int i = 0;
            while (isdigit(c) || c == '.') {
                token[i++] = c;
                c = fgetc(arquivo);
            }
            token[i] = '\0';
            if (strchr(token, '.') != NULL) {
                printf("Token: REAL_LITERAL\tLexema: %s\n", token);
            } else {
                printf("Token: INT_LITERAL\tLexema: %s\n", token);
            }
            ungetc(c, arquivo); // Coloca o caractere de volta no arquivo
        } else if (c == '"') {
            // String literal
            int i = 0;
            token[i++] = '"';
            c = fgetc(arquivo);
            while (c != EOF && c != '"') {
                token[i++] = c;
                c = fgetc(arquivo);
            }
            if (c == '"') {
                token[i++] = '"';
                token[i] = '\0';
                printf("Token: STRING_LITERAL\tLexema: %s\n", token);
            } else {
                printf("Token: DESCONHECIDO\t\tLexema: %s\n", token);
            }
        } else if (strchr(".,;(){}+- */& |~=< >", c) != NULL) {
            // Operadores e caracteres especiais
            token[0] = c;
            token[1] = '\0';
            int token_operador = obter_token_operador(token[0]);
            if (token_operador != -1) {
                printf("Token: OPERADOR_%s\t\tLexema: %s\n", token, token);
            } else {
                printf("Token: DESCONHECIDO\t\tLexema: %s\n", token);
            }
        }else {
            // Operadores compostos
            token[0] = c;
            token[1] = '\0';
            int token_operador = verificar_operador_composto(arquivo, token);
            if (token_operador != -1) {
                printf("Token: OPERADOR_%s\t\tLexema: %s\n", token, token);
            } else {
                printf("Token: DESCONHECIDO\t\tLexema: %s\n", token);
            }
        }
    }
}




int main() {
    // Abra o arquivo para leitura
    FILE *arquivo = fopen("seuarquivo.txt", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    // Chame a função de análise léxica
    analise_lexica(arquivo);
    // Feche o arquivo
    fclose(arquivo);

    return 0;
}