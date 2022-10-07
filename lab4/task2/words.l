%{
#include <limits.h>
enum { WORD = UCHAR_MAX + 1 };
%}


%%
[A-Za-z]{1,3} ;

[A-Za-z]+ {
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
        if (token == WORD) {
            printf("%s\n", yytext);
        }
    } while (token);
    return 0;
}
