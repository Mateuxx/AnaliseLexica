ch = prox_char(arquivo);
           if (c == '='&& ch == '='  ){
            token[0] = c;
            token[1] = ch;
            token[2] = '\0';
            fprintf(saida, "Token:  %s\t\tLexema: %s\n", token, token);

            if (c == '<'&& ch == '='  ){
            token[0] = c;
            token[1] = ch;
            token[2] = '\0';
            fprintf(saida, "Token:  %s\t\tLexema: %s\n", token, token);
           }
            else if (c == '>'&& ch == '='  ){
            token[0] = c;
            token[1] = ch;
            token[2] = '\0';
            fprintf(saida, "Token:  %s\t\tLexema: %s\n", token, token);
           }
            else if (c == '!'&& ch == '='  ){
            token[0] = c;
            token[1] = ch;
            token[2] = '\0';
            fprintf(saida, "Token:  %s\t\tLexema: %s\n", token, token);
            }
        }