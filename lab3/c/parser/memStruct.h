//
// Created by dmitriy on 9/24/22.
//

#ifndef LABS_MEMSTRUCT_H
#define LABS_MEMSTRUCT_H

#define NUM_BYTES 2

typedef struct MemStruct {
    int multiplier;

    int *value;

    int memsCount;
    struct MemStruct *mems;
} MemStruct;

int getBytes(MemStruct *mem);


#endif //LABS_MEMSTRUCT_H
