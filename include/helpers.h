/******************************************************************************
 *  dcanimator/include/helpers.h
 *
 */

#ifndef HELPERS_H
#define HELPERS_H

#include "config.h"

typedef struct __rgb {
    int                 red;
    int                 green;
    int                 blue
} RGB;

typedef struct __area {
    unsigned int        top;
    unsigned int        left;
    unsigned int        width;
    unsigned int        height;
} AREA;

RGB         RGB_get         (CONFIG *, const char *);

#endif
