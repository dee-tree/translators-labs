//
// Created by dmitriy on 9/24/22.
//

#include "memStruct.h"

int getBytes(MemStruct *mem) {
    if (mem->value) {
        return mem->multiplier * NUM_BYTES;
    }

    int totally = 0;
    for (int i = 0; i < mem->memsCount; i++) {
        totally += mem->multiplier * getBytes(mem->mems + i);
    }
    return totally;
}