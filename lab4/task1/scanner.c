//
// Created by dmitriy on 10/4/22.
//

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Print token as char or, if it's invisible, as decimal ASCII
 */
int prn_token(int token) {
    fprintf(stderr, isprint(token) ? "'%c' " : "\\%d ", token);
    return token;
}

long long power(int base, int power) {
    long long result = base;

    for (int i = 1; i < power; i++) {
        result *= base;
    }
    return result;
}

static long convert(char *yytext, int base, int size) {
    int x;
    long long mask = (power(2, size) - 1);
    int overflow = 0;

    x = strtol(yytext, NULL, base);

    if (errno == ERANGE || power(2, size - 1) <= x || x <= -power(2, size - 1)) {
        overflow = 1;
    }

    x = x & mask;

    if (overflow || x != (int) x) {
        fprintf(stderr, "\nFatal error: number %s is too big\n", yytext);
        exit(1);
    }
    return x;
}