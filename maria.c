#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"
#include <stdbool.h> 


struct SymbolEntry {
    char name[50]; // -> Nome do token
    int type;      // -> Símbolo
    struct SymbolEntry* next; // Para tratamento de colisões
};

// Variável global para controlar o número de linha
int linha_atual = 1;

int getLineNumber(void) {
    return linha_atual;
}

// Variável global
bool e = true;

// Função para verificar se a análise ainda está em execução
int isRunning(void) {
    return e ? 1 : 0;
}

// Tamanho da tabela de símbolos (um número primo)
#define TABLE_SIZE 101

// Tabela de símbolos (matriz de ponteiros para listas)
struct SymbolEntry* symbolTable[TABLE_SIZE];

// Função de hash simples para calcular o índice na tabela hash
int hash(char *name) {
    unsigned int hash = 0;
    while (*name) {
        hash = (hash * 31) + (*name++);
    }
    return hash % TABLE_SIZE;
}

// Função para inserir um símbolo na tabela de símbolos
void insertSymbol(char *name, int type) {
    int index = hash(name);
    
    // Criar uma nova entrada de símbolo
    struct SymbolEntry* newEntry = (struct SymbolEntry*) malloc(sizeof(struct SymbolEntry));
    strncpy(newEntry->name, name, sizeof(newEntry->name));
    newEntry->type = type;
    newEntry->next = NULL;
    
    // Verificar se já existe uma lista para esse índice
    if (symbolTable[index] == NULL) {
        symbolTable[index] = newEntry;
    } else {
        // Tratamento de colisões (adicionar na frente da lista)
        newEntry->next = symbolTable[index];
        symbolTable[index] = newEntry;
    }
}

int confere_simbolo(char *name) {
    int index = hash(name);
    
    // Verificar se já existe uma lista para esse índice
    struct SymbolEntry* entry = symbolTable[index];
    while (entry != NULL) {
        if (strcmp(entry->name, name) == 0) {
            return 1; // O símbolo já existe na tabela
        }
        entry = entry->next;
    }
    
    return 0; // O símbolo não existe na tabela
}

void printSymbolTable() {
    printf("Tabela de Símbolos:\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct SymbolEntry* entry = symbolTable[i];
        while (entry != NULL) {
            printf("Nome: %-20s\tTipo: %d\n", entry->name, entry->type);
            entry = entry->next;
        }
    }
    printf("-------------------------------------------------\n");
}

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

char obter_token_operador(char operador) {
    char operadores[] = {',', ';', '(', ')', '[', ']', '{', '}', '+', '-', '*', '/', '%', '&', '|', '~', '=', '<', '>', ':'};
    int tokens[] = {OPERADOR_VIRGULA, OPERADOR_PONTOVIRGULA, OPERADOR_LPAREN, OPERADOR_RPAREN, OPERADOR_COLCHETE_ESQ, OPERADOR_COLCHETE_DIR, OPERADOR_CHAVE_ESQ, OPERADOR_CHAVE_DIR, OPERADOR_SOMA, OPERADOR_SUBTRACAO, OPERADOR_MULTIPLICACAO, OPERADOR_DIVISAO, OPERADOR_MODULO, OPERADOR_E, OPERADOR_OU, OPERADOR_NAO, OPERADOR_ATRIBUICAO, OPERADOR_MENOR, OPERADOR_MAIOR, OPERADOR_DOISPONTOS};
    
    int num_operadores = sizeof(operadores) / sizeof(operadores[0]);
    
    for (int i = 0; i < num_operadores; i++) {
        if (operador == operadores[i]) {
            return tokens[i];
        }
    }

    return -1; // Operador desconhecido
}

char *getOperatorTokenName(int num) {
    switch (num) {
        case OPERADOR_VIRGULA:
            return "OPERADOR_VIRGULA";
        case OPERADOR_PONTOVIRGULA:
            return "OPERADOR_PONTOVIRGULA";
        case OPERADOR_LPAREN:
            return "OPERADOR_LPAREN";
        case OPERADOR_RPAREN:
            return "OPERADOR_RPAREN";
        case OPERADOR_COLCHETE_ESQ:
            return "OPERADOR_COLCHETE_ESQ";
        case OPERADOR_COLCHETE_DIR:
            return "OPERADOR_COLCHETE_DIR";
        case OPERADOR_CHAVE_ESQ:
            return "OPERADOR_CHAVE_ESQ";
        case OPERADOR_CHAVE_DIR:
            return "OPERADOR_CHAVE_DIR";
        case OPERADOR_ATRIBUICAO:
            return "OPERADOR_ATRIBUICAO";
        case OPERADOR_SOMA:
            return "OPERADOR_SOMA";
        case OPERADOR_SUBTRACAO:
            return "OPERADOR_SUBTRACAO";
        case OPERADOR_MULTIPLICACAO:
            return "OPERADOR_MULTIPLICACAO";
        case OPERADOR_DIVISAO:
            return "OPERADOR_DIVISAO";
        case OPERADOR_MODULO:
            return "OPERADOR_MODULO";
        case OPERADOR_MENOR:
            return "OPERADOR_MENOR";
        case OPERADOR_MAIOR:
            return "OPERADOR_MAIOR";
        case OPERADOR_E:
            return "OPERADOR_E";
        case OPERADOR_OU:
            return "OPERADOR_OU";
        case OPERADOR_NAO:
            return "OPERADOR_NAO";
        case OPERADOR_LE:
            return "OPERADOR_LE";
        case OPERADOR_GE:
            return "OPERADOR_GE";
        case OPERADOR_EQ:
            return "OPERADOR_EQ";
        case OPERADOR_PONTO:
            return "OPERADOR_PONTO";
        case OPERADOR_DOISPONTOS:
            return "OPERADOR_DOISPONTOS";
        default:
            return NULL; // Retorne NULL para valores desconhecidos
    }
}


int verificar_operador_composto(FILE *arquivo, char *token) {
    char proximo_caractere = fgetc(arquivo);
    int token_operador = -1;

    switch (token[0]) {
        case '=':
            if (proximo_caractere == '=') {
                strcat(token, "=");
                token_operador = OPERADOR_EQ;
            } else {
                // Não é um operador composto, volte um caractere para o arquivo
                ungetc(proximo_caractere, arquivo);
            }
    //         break;
    //     case '<':
    //         if (proximo_caractere == '=') {
    //             strcat(token, "=");
    //             token_operador = OPERADOR_LE;
    //         } else {
    //             // Não é um operador composto, volte um caractere para o arquivo
    //             ungetc(proximo_caractere, arquivo);
    //         }
    //         break;
    //     case '>':
    //         if (proximo_caractere == '=') {
    //             strcat(token, "=");
    //             token_operador = OPERADOR_GE;
    //         } else {
    //             // Não é um operador composto, volte um caractere para o arquivo
    //             ungetc(proximo_caractere, arquivo);
    //         }
    //         break;
    //     case '!':
    //         if (proximo_caractere == '=') {
    //             strcat(token, "=");
    //             token_operador = OPERADOR_NE;
    //         } else {
    //             // Não é um operador composto, volte um caractere para o arquivo
    //             ungetc(proximo_caractere, arquivo);
    //         }
    //         break;
    //     // Adicione outros casos para operadores compostos aqui
    //     default:
    //         // Não é um operador composto, não faça nada
    //         break;
    // }

    return token_operador;
}
}

void to_upper(char* string)
{
    const char OFFSET = 'a' - 'A';
    while (*string)
    {
        *string = (*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;
        string++;
    }
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
            return LIT_STRING;
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

char prox_char(FILE* file){
    char ch;
    do {
        ch = fgetc(file);
    } while(ch == ' ' || ch == '\t' || ch == '\n');
        if(ch=='\n') {
            linha_atual++;
        }
    return ch;
}

void grava_token(FILE *saida, char *token, char *lexema) {
    fprintf(saida, "Token:%-20s\tLexema: %s\n", token, lexema);
}

void analise_lexica(FILE *arquivo, FILE *saida) {
    char token[100];
    int c;
    char ch; //variavel para usar para pegar o proximoChar quando necessario
    int comentario = 0; // Variável para controlar se estamos dentro de um comentário de bloco
    int cont = 0; //contador

    while ((c = prox_char(arquivo)) != EOF) {
          
        if (c == '\n') {
            // Atualize o número da linha quando encontrar uma nova linha
            linha_atual++;
        }
        if (c == '/') {
            int c2 = prox_char(arquivo);
            if (c2 == '/') {
                // É um comentário de linha, ignorar até a quebra de linha
                while ((c = prox_char(arquivo)) != EOF && c != '\n');
            } else if (c2 == '*') {
                // É o início de um comentário de bloco, marcar como tal
                comentario = 1;
                while (1) {
                    c = prox_char(arquivo);
                    if (c == EOF) {
                        e = false;
                        //fprintf(saida, "Erro: Comentário de bloco não fechado.\n");
                        break;
                    }
                    if (c == '*') {
                        int c3 = prox_char(arquivo);
                        if (c3 == '/') {
                            comentario = 0; // Fim do comentário de bloco
                            break;
                        } else {
                            ungetc(c3, arquivo);
                        }
                    }
                }
            } else {
                // Não é um comentário, tratar como caractere normal
                ungetc(c2, arquivo);
                token[0] = c;
                token[1] = '\0';
                int token_operador = obter_token_operador(token[0]);
                if (token_operador != -1) {
                    fprintf(saida, "Token: OPERADOR_%s\t\tLexema: %s\n",token, token);
                } else {
                    fprintf(saida, "Token: DESCONHECIDO\t\tLexema: %s\n", token);
                    if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
                }
            }
        } else if (comentario) {
            // Estamos dentro de um comentário de bloco, ignorar até encontrar o fim
            if (c == '*') {
                int c2 = prox_char(arquivo);
                if (c2 == '/') {
                    comentario = 0; // Fim do comentário de bloco
                } else {
                    ungetc(c2, arquivo);
                }
            }
        } else if (isspace(c)) {
            // Ignorar espaços em branco
            continue;
        } else if (isalpha(c) || c == '_') {
            // Identificador
            int i = 0;
            while (isalnum(c) || c == '_') {
                token[i++] = c;
                c = prox_char(arquivo);
            }
            token[i] = '\0';
            if (eh_palavra_reservada(token)) {
                to_upper(token);
                fprintf(saida, "Token: KW_%-20s\t\tLexema: %s\n", token, token);
            } else {
                grava_token(saida, "TW_IDENTIFIER",token);
                // Adicione o identificador à tabela de símbolos
                if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
            }
            ungetc(c, arquivo); // Coloca o caractere de volta no arquivo
        } else if (isdigit(c)) {
            // Literal inteiro ou real
            int i = 0;
            while (isdigit(c) || c == '.') {
                token[i++] = c;
                c = prox_char(arquivo);
            }
            token[i] = '\0';
            if (strchr(token, '.') != NULL) {
                fprintf(saida, "Token: LIT_REAL\tLexema: %s\n", token);
                if (!confere_simbolo(token)) { insertSymbol(token, LIT_REAL);}
            } else {
                grava_token(saida, "LIT_INT", token);
                if (!confere_simbolo(token)) { insertSymbol(token, LIT_INT);}
            }
            ungetc(c, arquivo); // Coloca o caractere de volta no arquivo
        } 
        //LITCHAR
            else if (c == '\'') {
            token[0] = c;
            ch = prox_char(arquivo);
            cont = 1; // Inicialize cont para 1
            while (ch != '\'' && ch != '\n') { // Verifique se não é um apóstrofo simples ou uma nova linha
                token[cont] = ch;
                cont++;
                ch = prox_char(arquivo);
            }
            if (ch == '\'') {
                // Verificou-se que o caractere literal foi fechado corretamente
                token[cont] = ch;
                token[cont+1] ='\0';// Adicione o apóstrofo simples de fechamento ao token
                grava_token(saida, "LIT_CHAR", token);
            } else {
                // Tratamento de erro: caractere literal não foi fechado
                // Você pode lançar um erro ou tomar alguma outra ação apropriada aqui.
            }
        }
         else if (c == '"') {
            int i = 0;
            token[i++] = '"';
            c = prox_char(arquivo);
            while (c != EOF && c != '"') {
                token[i++] = c;
                c = prox_char(arquivo);
            }
            if (c == '"') {
                token[i++] = '"';
                token[i] = '\0';
                grava_token(saida, "LIT_STRING",token);
                if (!confere_simbolo(token)) {
                    insertSymbol(token, LIT_STRING);
                }
            } else {
                token[i++] = '\0';
                grava_token(saida, "DESCONHECIDO",token);
            }
        } 

        else if (c == '.' || c == ',' || c == ';' || 
                    c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || 
                    c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&' || c == '|' ||
                     c == '~' || c == ':' ) {
            //operadores simples
            token[0] = c;
            token[1] = '\0';
            int token_operador = obter_token_operador(token[0]);
            char * tokenName = getOperatorTokenName(token_operador);
            if (token_operador != -1) {
                grava_token(saida, tokenName, token);
            } else {
                grava_token(saida, "DESCONHECIDO", token);
                if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
                }
            }
            //operador Composto == 
            else if (c == '='){
                ch = prox_char(arquivo);
                if(ch == '=') {
                     token[0] = c;
                     token[1] = ch;
                     grava_token(saida, "OPERATOR_GE", token);
                }else{
                ungetc(ch, arquivo);
                token[0] = c;
                token[1] = '\0';
                int token_operador = obter_token_operador(token[0]);
                char * tokenName = getOperatorTokenName(token_operador);
                if (token_operador != -1) {
                    grava_token(saida, tokenName, token);
                } else {
                    grava_token(saida, "DESCONHECIDO", token);
                    if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
                    }
                    }
            }
            //operador Composto >= 
            else if (c == '>'){
                ch = prox_char(arquivo);
                if(ch == '=') {
                    token[0] = c;
                    token[1] = ch;
                    token[2] = '\0';
                    grava_token(saida, "OPERATOR_GE", token);
                }else {
                //operadore simples >
                ungetc(ch, arquivo);
                token[0] = c;
                token[1] = '\0';
                int token_operador = obter_token_operador(token[0]);
                char * tokenName = getOperatorTokenName(token_operador);
                if (token_operador != -1) {
                    grava_token(saida, "OPERADOR_MAIOR", token);
                } else {
                    grava_token(saida, "DESCONHECIDO", token);
                    if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
                    }
                }       
            }
             //operador Composto <= 
            else if (c == '<'){
                ch = prox_char(arquivo);
                if(ch == '=') {
                    token[0] = c;
                    token[1] = ch;
                    token[2] = '\0';
                    grava_token(saida, "OPERATOR_LE", token);
                }else {
                //operadore simples <
                token[0] = c;
                token[1] = '\0';
                int token_operador = obter_token_operador(token[0]);
                char * tokenName = getOperatorTokenName(token_operador);
                if (token_operador != -1) {
                    grava_token(saida, "OPERATOR_MENOR", token);
                } else {
                    grava_token(saida, "DESCONHECIDO", token);
                    if (!confere_simbolo(token)) { insertSymbol(token, TOKEN_ERROR);}
                    }
                }       
            }
            //Operador composto !=
               else if (c == '!'){
                ch = prox_char(arquivo);
                if(ch == '=') {
                    token[0] = c;
                    token[1] = ch;
                    token[2] = '\0';
                    grava_token(saida, "OPERATOR_DIF", token);
                } else{
                    grava_token(saida, "DESCONHECIDO", token);
                }    
            }
            if (c == EOF) {
                e = false;
            }            
    }
        

    // Verificar se o código terminou com um comentário de bloco aberto
    if (comentario) {
        fprintf(saida, "Erro: Comentário de bloco não fechado.\n");
    }

    //função isRUnning
    if (isRunning()) {
        printf("A análise está em execução.\n");
    } else {
        printf("A análise foi concluída.\n");
    }

}

int main() {
    FILE *arquivo = fopen("seuarquivo.txt", "r");
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