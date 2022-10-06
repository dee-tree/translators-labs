%token   NUMBER MONTH SEPARATOR
%start   date

%%
date :   MONTH NUMBER SEPARATOR NUMBER
%%
