#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "lexico.h"

/*
Aluno: Mateus Mota Nobrega - 21953021
Aluno: Gabriel César Tavares Ferreira - 21854868
*/

#define MAX_SYMBOLS 100

typedef struct {
    char* name;
    int type;
} Symbol;

Symbol symbol_table[MAX_SYMBOLS];
int num_symbols = 0;

void add_symbol(char* name, int type) {
    if (num_symbols >= MAX_SYMBOLS) {
        printf("Symbol table overflow!\n");
        exit(1);
    }
    symbol_table[num_symbols].name = strdup(name);
    symbol_table[num_symbols].type = type;
    num_symbols++;
}
int lookup_symbol(char* name) {
    int i;
    for (i = 0; i < num_symbols; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].type;
        }
    }
    return ID;
}

// int verificaToken(Token token){
//     if(token.type == 0){
//         return 1;
//     }
//     else{
//         return 0;
//     }
// } 

void grava_token(FILE *file, Token token){
    fprintf(file, "<%s, %s>\n", token.name, token.value);
}

char prox_char(FILE* file){
    char ch;
    do {
        ch = fgetc(file);
    } while(ch == ' ' || ch == '\t' || ch == '\n');
    return ch;
}

Token analex(char c, FILE *file){
    Token token;
    char buffer[100] = {0};
    char ch;
    buffer[0] = c;
    int i = 1;
    //INT E FLOAT
    if(isdigit(c)){
        while(isdigit(ch = prox_char(file))){
            buffer[i] = ch;
            i++;
        }
        if(ch == '.'){
            buffer[i] = ch;
            i++;
            if(!isdigit(ch = fgetc(file))){
                token.type = 0;
                token.value = strdup(buffer);
                token.name = "ERRO Numero Real Incompleto";
            }
            else{
                buffer[i] = ch;
                i++;
                while(isdigit(ch = prox_char(file))){
                buffer[i] = ch;
                i++;
                }            
                token.type = NUM_FLOAT;
                token.value = strdup(buffer);
                token.name = "NUM_FLOAT";
            }
            return token;
        }
        ungetc(ch, file);
        token.type = NUM_INT;
        token.value = strdup(buffer);
        token.name = "NUM_INT"; 
        return token;
    }
    //STRINGS
    else if(c == '"'){
        while((ch = fgetc(file)) != EOF){     
            if(ch == '"'){
                token.type = STRINGS;
                token.value = strdup(buffer);
                token.name = "STRINGS";
                break;
            }
            else if(ch == ' '){ 
                token.type = 0;
                token.value = strdup(buffer);
                token.name = "ERRO string incompleta";
                break;
            }
            else{
                buffer[i] = ch;
                i++;   
            }
        }
        return token;

    }
    //CARACTERE
    else if(c == '\''){
        ch = prox_char(file);
        buffer[i] = ch;
        i++;
        if((ch = prox_char(file)) == '\''){
            buffer[i] = ch;
            token.type = CARACTERE;
            token.value = strdup(buffer);
            token.name = "CARACTERE";
            return token;
        }
        else{
            buffer[i] = ch;
            token.type = 0;
            token.value = strdup(buffer);
            token.name = "CARACTERE INCOMPLETO";
            return token;
        }
    }

    // OPERADORES 
    else if (c == '/') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPATRIBDIV;
            token.value = strdup(buffer);
            token.name = "OPATRIBDIV";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPDIV;
            token.value = strdup(buffer);
            token.name = "OPDIV";
            return token;
        }
    } 
    else if (c == '*') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPATRIBMULT;
            token.value = strdup(buffer);
            token.name = "OPATRIBMULT";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPMULT;
            token.value = strdup(buffer);
            token.name = "OPMULT";
            return token;
        }
    } 
    else if (c == '=') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPIG;
            token.value = strdup(buffer);
            token.name = "OPIG";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPATRIB;
            token.value = strdup(buffer);
            token.name = "OPATRIB";
            return token;
        }
    } 
    else if( c == '+'){
        if((ch = fgetc(file)) == '+'){
            buffer[i] = ch;
            i++;
            token.type = OPINC;
            token.value = strdup(buffer);
            token.name = "OPINC";
            return token;
        }
        else{
            ungetc(ch, file);
        }
        if((ch = prox_char(file)) == '='){
            buffer[i] = ch;
            i++;
            token.type = OPDATRIBAD;
            token.value = strdup(buffer);
            token.name = "OPATRIBAD";
            return token;
        }
        else{
            ungetc(ch, file);
            token.type = OPAD;
            token.value = strdup(buffer);
            token.name = "OPAD";
            return token;
        }
    }
    else if (c == '%') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPATRIBMOD;
            token.value = strdup(buffer);
            token.name = "OPATRIBMOD";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPMOD;
            token.value = strdup(buffer);
            token.name = "OPMOD";
            return token;
        }
    }
    else if (c == '<') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPMENIG;
            token.value = strdup(buffer);
            token.name = "OPMENIG";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPREMEN;
            token.value = strdup(buffer);
            token.name = "OPREMEN";
            return token;
        }
    }
    else if (c == '>') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPMAIIG;
            token.value = strdup(buffer);
            token.name = "OPMAIIG";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPREMAI;
            token.value = strdup(buffer);
            token.name = "OPREMAI";
            return token;
        }
    }
    else if (c == '&') {
        if ((ch = prox_char(file)) == '&') {
            buffer[i] = ch;
            i++;
            token.type = OPAND;
            token.value = strdup(buffer);
            token.name = "OPAND";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPANDBB;
            token.value = strdup(buffer);
            token.name = "OPANDBB";
            return token;
        }
    }
    else if (c == '!') {
        if ((ch = prox_char(file)) == '=') {
            buffer[i] = ch;
            i++;
            token.type = OPDIF;
            token.value = strdup(buffer);
            token.name = "OPDIF";
            return token;
        } else {
            ungetc(ch, file);
            token.type = OPNOT;
            token.value = strdup(buffer);
            token.name = "OPNOT";
            return token;
        }
    }
    else if(c == '|'){
        if((ch = prox_char(file)) == '|'){
            buffer[i] = ch;
            i++;
            token.type = OPOR;
            token.value = strdup(buffer);
            token.name = "OPOR";
            return token;
        }
        else{
            ungetc(ch, file);
            token.type = ID;
            token.value = strdup(buffer);
            token.name = "ID";
            return token;
        }
    }
    else if(c == '.'){
        token.type = OPSTRUC;
        token.value = strdup(buffer);
        token.name = "OPSTRUC";
        return token;
    }
    else if(c == '?'){
        if((ch = prox_char(file)) == ':'){
            buffer[i] = ch;
            i++;
            token.type = OPTERN;
            token.value = strdup(buffer);
            token.name = "OPTERN";
            return token;
        }
        else{
            ungetc(ch, file);
            token.type = ID;
            token.value = strdup(buffer);
            token.name = "ID";
            return token;
        }
    }
    else if(c == '-'){
        if((ch = prox_char(file)) == '>'){
            buffer[i] = ch;
            i++;
            token.type = OPPONTSTRUC;
            token.value = strdup(buffer);
            token.name = "OPPONTSTRUC";
            return token;
        }
        else{
            ungetc(ch, file);
        }
        if((ch = prox_char(file)) == '='){
            buffer[i] = ch;
            i++;
            token.type = OPATRIBSUB;
            token.value = strdup(buffer);
            token.name = "OPATRIBSUB";
            return token;
        }
        else{
            ungetc(ch, file);
            token.type = OPSUB;
            token.value = strdup(buffer);
            token.name = "OPSUB";
            return token;
        }
    }
    //PONTUAÇÕES
    else if(c == ','){
        token.type = VIRG;
        token.value = strdup(buffer);
        token.name = "VIRG";
        return token;
    }
    else if(c == ';'){
        token.type = PV;
        token.value = strdup(buffer);
        token.name = "PV";
        return token;
    }
    else if(c == '('){
        token.type = OPENPAR;
        token.value = strdup(buffer);
        token.name = "OPENPAR";
        return token;
    }
    else if(c == ')'){
        token.type = CLOSEPAR;
        token.value = strdup(buffer);
        token.name = "CLOSEPAR";
        return token;
    }
    else if(c == '{'){
        token.type = OPENCHAV;
        token.value = strdup(buffer);
        token.name = "OPENCHAV";
        return token;
    }
    else if(c == '}'){
        token.type = CLOSECHAV;
        token.value = strdup(buffer);
        token.name = "CLOSECHAV";
        return token;
    }
    else if(c == '['){
        token.type = OPENCOLC;
        token.value = strdup(buffer);
        token.name = "OPENCOLC";
        return token;
    }
    else if(c == ']'){
        token.type = CLOSECOLC;
        token.value = strdup(buffer);
        token.name = "CLOSECOLC";
        return token;
    }

    //PALAVRAS RESERVADAS
    else{
        while((ch = fgetc(file)) != EOF){
                buffer[i] = ch;
                i++;
                if(i == 4 && strcmp("void", buffer) == 0){
                    token.type = VOID;
                    token.value = strdup(buffer);
                    token.name = "VOID";
                    break;
                }
                else if(i == 4 && strcmp("main", buffer) == 0){
                    token.type = MAIN_PR;
                    token.value = strdup(buffer);
                    token.name = "MAIN_PR";
                    break;
                }
                else if(i == 3  && strcmp("int ", buffer) == 0){
                    token.type = TIPO_INT;
                    token.value = strdup(buffer);
                    token.name = "TIPO_INT";
                    break;
                }
                else if(i == 5 && strcmp("float", buffer) == 0){
                    token.type = TIPO_FLOAT;
                    token.value = strdup(buffer);
                    token.name = "TIPO_FLOAT";
                    break;
                }
                else if(i == 4 && strcmp("char", buffer) == 0){
                    token.type = TIPO_CHAR;
                    token.value = strdup(buffer);
                    token.name = "TIPO_CHAR";
                    break;
                }
                else if(i == 4 && strcmp("bool", buffer) == 0){
                    token.type = BOOLEANO;
                    token.value = strdup(buffer);
                    token.name = "BOOLEANO";
                    break;
                }
                else if(i == 2 && strcmp("if", buffer) == 0){
                    token.type = IF_PR;
                    token.value = strdup(buffer);
                    token.name = "IF_PR";
                    break;
                }
                else if(i == 4 && strcmp("else", buffer) == 0){
                    token.type = ELSE_PR;
                    token.value = strdup(buffer);
                    token.name = "ELSE_PR";
                    break;
                }
                else if(i == 3 && strcmp("for", buffer) == 0){
                    token.type = FOR_PR;
                    token.value = strdup(buffer);
                    token.name = "FOR_PR";
                    break;
                }
                else if(i == 5 && strcmp("while", buffer) == 0){
                    token.type = WHILE_PR;
                    token.value = strdup(buffer);
                    token.name = "WHILE_PR";
                    break;
                }
                else if(i == 2 && strcmp("do", buffer) == 0){
                    token.type = DO_PR;
                    token.value = strdup(buffer);
                    token.name = "DO_PR";
                    break;
                }
                else if(i == 6 && strcmp("return", buffer) == 0){
                    token.type = RETURN_PR;
                    token.value = strdup(buffer);
                    token.name = "RETURN_PR";
                    break;
                }
                else if(i == 5 && strcmp("break", buffer) == 0){
                    token.type = BREAK_PR;
                    token.value = strdup(buffer);
                    token.name = "BREAK_PR";
                    break;
                }
                else if(i == 8 && strcmp("continue", buffer) == 0){
                    token.type = CONTINUE_PR;
                    token.value = strdup(buffer);
                    token.name = "CONTINUE_PR";
                    break;
                }
                else if(i == 4 && strcmp("goto", buffer) == 0){
                    token.type = GOTO_PR;
                    token.value = strdup(buffer);
                    token.name = "GOTO_PR";
                    break;
                }
                else if(i == 4 && strcmp("true", buffer) == 0){
                    token.type = TRUE_PR;
                    token.value = strdup(buffer);
                    token.name = "TRUE_PR";
                    break;
                }
                else if(i == 5 && strcmp("false", buffer) == 0){
                    token.type = FALSE_PR;
                    token.value = strdup(buffer);
                    token.name = "FALSE_PR";
                    break;
                }
                else if(isspace(ch)){
                    token.type = ID;
                    token.value = strdup(buffer);
                    token.name = "ID";
                    break;
                }
                else{
                    token.type = ID;
                    token.value = strdup(buffer);
                    token.name = "ID";
                }
            }
            return token;
            }
}

