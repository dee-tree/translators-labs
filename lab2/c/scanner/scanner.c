#include <stdio.h>
#include <ctype.h>
// #include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/* ------- Global data ------- */

char yytext[1024];
int yyleng;
YYSTYPE yylval;

/* ------- Local macros and functions ------- */

#define unput(c) { ungetc(c, stdin); yytext[--yyleng] = 0; }

/*
 *  Read and save a char from stdio, used by __yylex()
 */
static int input() {
    int c;

    c = getchar();
    if (c == EOF) c = 0;
    yytext[yyleng++] = c;
    return c;
}

/* way to get binary representation of in */
const char *int_to_binary(int x) {
    static char b[33];
    b[0] = '\0';

    long z;
    for (z = 4294967296; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

long long power(int base, int power) {
    long long result = base;

    for (int i = 1; i < power; i++) {
        result *= base;
    }
    return result;
}

const char *long_to_binary(long long x) {
    static char b[65];
    b[0] = '\0';

    unsigned long long z;
    for (z = 1llu << 63; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

/*
 * Convert yytext to yylval with overflow checking
 */
static void convert(int base, int size) {
    long long x;
    long long mask = (power(2, size) - 1);
    int overflow = 0;

    x = strtoll(yytext, NULL, base);

    if (errno == ERANGE || power(2, size - 1) <= x || x <= -power(2, size - 1)) {
        overflow = 1;
    }

    x = x & mask;
    yylval = x;

    if (overflow || yylval != (int) x) {
        fprintf(stderr, "\nFatal error: number %s is too big\n", yytext);
        exit(1);
    }
}

static int isBinaryDigit(int c) {
    return c == '0' || c == '1';
}


/*
 *  Get next token from stdio, used by yylex()
 */
static int __yylex() {
    int c, state;


    for (state = 0;;) {
        c = input();
        switch (state) {
            case 0:               /* skip whitespaces */
                if (isspace(c)) {
                    yyleng = 0;
                    break;
                }

                if (isalpha(c)) {

                    if (c == 'L') { /*  long num   */
                        state = 5; /* handle as long num */
                        break;
                    }

                    state = 1;      /* treat next chars as Identifier */
                    break;
                }
                if (isdigit(c)) {

                    if (isBinaryDigit(c)) {
                        state = 3; /*  detect next binary digits  */
                        break;
                    }

                    state = 2;      /* treat next chars as Number */
                    break;
                }

                if (c == '-') {         /* unary minus before num ? */
                    c = input();
                    if (isdigit(c)) {
                        state = 2;      /* num! */
                        break;
                    } else {
                        unput(c);
                        return '-';
                    }
                }

                return c;           /* return one-char Literal */

            case 1:               /* build Identifier */
                if (!isalnum(c)) {
                    unput(c);
                    return ID;
                }
                break;

            case 2:               /* build Number */
                if (!isdigit(c)) {
                    convert(10, SHORT);
                    unput(c);
                    return NUM;
                }
                break;

            case 3:             /* binary digits  */
                if (!isBinaryDigit(c)) {
                    if (c != 'x') {
                        unput(c);
                        state = 2;  /* !not binary num -> go to decimal num */
                        break;
                    }

                    state = 4; /* confirm that number is binary */
                }

                break;

            case 4:             /*  confirm base of number after "x" suffix  */

                if (c == 'b') {
                    /* bingo! binary number */
                    convert(2, LONG);
                    return NUM;
                }

                unput(c);
                unput('x');
                state = 2; /*  just decimal number */

                break;

            case 5:      /*  long num  */
                if (c != '#') {
                    state = 1; /*  handle as identifier */
                    break;
                }

                c = input();
                if (!isdigit(c) && c != '-') {
                    unput(c);
                    unput('#');
                    state = 1;
                    break;
                }

                if (c == '-') { /* unary minus */
                    c = input();
                    if (isdigit(c)) {
                        state = 6;      /* num! */
                        break;
                    } else {
                        unput(c);
                        unput('-');
                        unput('#');
                        state = 1; /* handle ID */
                        break;
                    }
                }

                state = 6;

                break;

            case 6:   /*  read digits of long num */
                if (!isdigit(c)) {
                    yyleng -= 2;
                    memmove(yytext,  yytext + 2, yyleng);
                    convert(10, LONG);
                    memmove(yytext + 2, yytext, yyleng + 2);
                    yyleng += 2;
                    yytext[0] = 'L'; yytext[1] = '#';
                    unput(c);
                    return NUM;
                }

                break;

            default:
                fprintf(stderr, "\nFatal error: yylex is in unknown state\n");
                exit(1);
        }
    }
}


/* ------- Global functions ------- */

/*
 *  Get next token from stdin
 */
int yylex() {
    int token;

    yyleng = 0;
    token = __yylex();
    yytext[yyleng] = 0;     /* zero-end the string */
    return token;
}

/* 
 *  Print token as char or, if it's invisible, as decimal ASCII 
 */
int prn_token(int token) {
    fprintf(stderr, isprint(token) ? "'%c' " : "\\%d ", token);
    return token;
}   


