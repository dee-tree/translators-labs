/*
 *  dw 10, 1k dup (?, 4, 18 dup 0, 2k)
 *  (define word => 2 bytes)
 *  Task: parse command, calculate number of bytes to be allocated
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "memStruct.h"

/*
 * GRAMMAR
 *
 * command = dw <arglist>
 * arglist = arg [COMMA arg]*
 * arg = NUM | DUP_NUM
 * DUP_NUM = NUM DUP DUP_ARGS
 * DUP_ARGS = (NUM) | (LPAREN arg [COMMA arg]* RPAREN)
 */

MemStruct parseDupArgs(int (*readToken)(int *), int multiplier, int inheritance);
MemStruct parseDwArg(int (*readToken)(int *), int inheritance);

int token;
int getValueOfNum() {
    return token == NUM ? yylval : 0;
}


int chk_single_num[] = {NUM, QUEST, 0};
int chk_single_num_or_open[] = {NUM, QUEST, LPAREN, 0};
int chk_comma[] = {COMMA, 0 };
int chk_comma_or_dup_or_close[] = {COMMA, DUP, RPAREN, 0};
int chk_comma_or_dup[] = {COMMA, DUP, 0};
int chk_comma_or_close[] = {COMMA, RPAREN, 0};
int chk_close[] = { RPAREN, 0};

int chk_dw[] = {DW, 0};


int main(void) {
    unsigned long long totalBytes = 0;

    if (!(token = rd_token(chk_dw))) {         /* get dw or EOF */
        printf("Empty command\n");     /* EOF is OK here */
        return 0;
    }

    for (totalBytes = 0;;) {
        MemStruct mem = parseDwArg(rd_token, 0);
        totalBytes += getBytes(&mem);

        if (token != COMMA && !(token = rd_token(chk_comma))) {
            break;
        }

    }

    printf("no. of reserved bytes = %lld\n", totalBytes);
    return 0;
}


MemStruct parseDwArg(int (*readToken)(int *), int inheritance) {
    if (!(token = readToken(chk_single_num))) {         /* get number or EOF */
        bad_eof();          /* EOF isn't OK here */
    }
    int value = getValueOfNum();

    if (token == QUEST) {

        if (!(token = (inheritance > 0 ? readToken(chk_comma_or_close) : readToken(chk_comma)))) {
            if (inheritance > 0)
                bad_eof();
        }

        MemStruct mem = { 1, &value, 0, NULL };
        return mem;
    } else {
        // may be just NUM or dup expression

        if (!(token = (inheritance > 0 ? readToken(chk_comma_or_dup_or_close) : readToken(chk_comma_or_dup) ))) {
            if (inheritance > 0)
                bad_eof();
        }

        if (token != DUP) {
            MemStruct mem = { 1, &value, 0, NULL };
            return mem;
        }

        MemStruct mem = parseDupArgs(readToken, value, inheritance);

        if (! (token = inheritance > 0 ? readToken(chk_comma_or_close) : readToken(chk_comma))) {
            if (inheritance > 0)
                bad_eof();
        }

        return mem;
    }
}

MemStruct parseDupArgs(int (*readToken)(int *), int multiplier, int inheritance) {
    if (!(token = readToken(chk_single_num_or_open))) {
        bad_eof();
    }

    if (token == NUM || token == QUEST) {
        int value = getValueOfNum();
        MemStruct mem = {multiplier, &value, 0, NULL};

        return mem;
    }

    // here was lparen

    int argsCount = 0;
    MemStruct *mems = malloc(sizeof(MemStruct));

    while (token != RPAREN) {
        mems[argsCount++] = parseDwArg(readToken, inheritance + 1);
        if ((token == NUM || token == QUEST) && !(token = readToken(chk_close))) {
            bad_eof();
        }

        MemStruct *new = realloc(mems, (argsCount + 1) * sizeof(MemStruct));
        mems = new;
    }

    // rparen met here

    MemStruct mem = { multiplier, NULL, argsCount, mems };
    return mem;

}