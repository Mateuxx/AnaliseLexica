#ifndef LEXICO_H
#define LEXICO_H


/*
Aluno: Mateus Mota Nobrega - 21953021
Aluno: Gabriel César Tavares Ferreira - 21854868
*/

#include "stdio.h"

#define VOID 1 
#define TIPO_INT 2 
#define TIPO_FLOAT 3 
#define TIPO_CHAR 4 
#define BOOLEANO 5 
#define IF_PR 6 
#define ELSE_PR 7 
#define FOR_PR 8 
#define WHILE_PR 9 
#define DO_PR 10 
#define RETURN_PR 11 
#define BREAK_PR 12 
#define CONTINUE_PR 13 
#define GOTO_PR 14 
#define TRUE_PR 15 
#define FALSE_PR 16 
#define MAIN_PR 56 

#define OPAD 17 
#define OPSUB 18 
#define OPMULT 19 
#define OPDIV 20 
#define OPMOD 21 
#define OPTERN 22 
#define OPNOT 23 
#define OPANDBB 24 
#define OPSTRUC 25 
#define OPPONTSTRUC 26 
#define OPREMEN 27 
#define OPREMAI 28 
#define OPIG 29 
#define OPDIF 30 
#define OPMENIG 31 
#define OPMAIIG 32 
#define OPATRIB 33
#define OPDATRIBAD 34 
#define OPATRIBSUB 35 
#define OPATRIBMULT 36 
#define OPATRIBDIV 37 
#define OPATRIBMOD 38 
#define OPINC 39 
#define OPAND 40 
#define OPOR 41 

#define VIRG 42 
#define PV 43 
#define OPENPAR 44 
#define CLOSEPAR 45 
#define OPENCHAV 46 
#define CLOSECHAV 47 
#define OPENCOLC 48 
#define CLOSECOLC 49 

#define NUM_INT 50 
#define NUM_FLOAT 51 
#define CARACTERE 52 
#define STRINGS 53 
#define VAR_BOOL 54 
#define ID 55 


typedef struct token Token;
struct token{
    char *value;
    int type;
    char *name;
};

int verificaToken(Token token); //verifica se o token tem um tipo válido, se não tiver ele para o programa
void grava_token(FILE *file, Token token);//grava um token no arquivo de saída
char prox_char(FILE* file); //lê um caractere do arquivo por vez
Token analex(char c, FILE *file); //analisa o caractere passado e cria o token correspondente

void add_symbol(char* name, int type);
int lookup_symbol(char* name);

#endif /* LEXICO_H */