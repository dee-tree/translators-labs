%{

#include "scanner.c"
#include <limits.h>

long yylval;

enum { NUM = UCHAR_MAX + 1, ID };

%}

ID       [A-Za-z_][A-Za-z_\d]*
NUM      [+-]?[0-9]+
LONG_NUM L#{NUM}

%%
{ID} {
    return ID;
}

{NUM} {
    yylval = convert(yytext, 10, 16);
    return NUM;
}

{LONG_NUM} {
    yylval = convert(yytext + 2, 10, 32);
    return NUM;
}

. return yytext[0];

%%

#ifndef yywrap
int yywrap() { return 1; }
#endif    

int main () {
    int token;
    do {
    token = yylex();
    fprintf(stderr, "token\t");
            prn_token(token);
    		fprintf(stderr, "\tyytext %s\tyyleng %d\tyylval %d\n", yytext, yyleng, yylval);
    } while (token);
    return 0;
}
