#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "mmu.h"

void printb(const char *title, int val, int nbits)
{
    char buf[nbits + 1];
    for (int i = nbits - 1; i >= 0; i--) {
        buf[i] = val & 1 ? '1' : '0';
        val >>= 1;
    }
    buf[nbits] = '\0';
    printf("%s: %s\n", title, buf);
}

long parse_addr(char *hex, int nbits)
{
    char *endptr;
    long addr;

    if (hex[strlen(hex) - 1] == '\n') {
        hex[strlen(hex) - 1] = 0;
    }

    addr = strtol(hex, &endptr, 16);
    if (addr < 0 || addr >= pow(2, nbits) || *endptr != '\0') {
        return -1;
    }

    return addr;
}