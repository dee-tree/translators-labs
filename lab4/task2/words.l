%{
#include <limits.h>
enum { WORD = UCHAR_MAX + 1 };
%}

WORD [A-Za-z]+

%%
{WORD} {
    return WORD;
}

[ \t\n]+          /* do nothing */
. return yytext[0];

%%

#ifndef yywrap
int yywrap() { return 1; }
#endif    

int main () {
    int token;
    do {
        token = yylex();
        if (token == WORD && yyleng > 3) {
            printf("%s\n", yytext);
        }
    } while (token);
    return 0;
}
