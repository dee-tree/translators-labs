%{
#include <stdlib.h>
#include "memStruct.h"
#include "y.tab.h"

//#define YYSTYPE int
//extern YYSTYPE yylval;    /* value of numeric token */
%}
 
%%

"(" { return LPAREN; }
")" { return RPAREN; }
"?" { return QUEST; }
"," { return COMMA; }

dw { return DW; }
dup { return DUP; }


[0-9]+k?     { yylval.ival = atoi(yytext); if (yytext[yyleng - 1] == 'k') yylval.ival *= 1024; return NUM; }
<<EOF>>  { return 0; }
(.|\n)     ;
%%

#ifndef yywrap
int yywrap () { return 1; }
#endif
