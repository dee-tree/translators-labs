%{
#include "memStruct.h"
%}

%union
{
    int ival;
    int mem;
    //MemStruct mem;
};

%token <ival> NUM DW DUP LPAREN RPAREN QUEST COMMA
%type <mem> dw_arg
%type <mem> dw_arglist
%type <mem> dup_arglist
%type <mem> dup_expr
%start dw_statement

%%

dw_statement: DW dw_arglist { printf("no. of reserved bytes = %d\n", $2); }
              | /* empty command */ { printf("Empty command\n"); }

dw_arglist: dw_arg { $$ = $1; }
            | dw_arg
              COMMA
              dw_arglist  { $$ = $1 + $3; }

dw_arg: NUM { $$ = 2; }
        | QUEST { $$ = 2; }
        | dup_expr { $$ = $1; }


dup_expr: NUM DUP dup_arglist { $$ = $1 * $3; }

dup_arglist: NUM { $$ = 2; }
            | QUEST { $$ = 2; }
            | LPAREN
              dw_arglist
              RPAREN { $$ = $2; }

%%

