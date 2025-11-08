/******************************************************************************
 *  dcanimator/include/component.h
 *
 */
#define _GNU_SOURCE

#include <wchar.h>
#include <notcurses/notcurses.h>

#ifndef COMPONENT_H
#define COMPONENT_H

#include "app.h"
#include "helpers.h"

#define COMPONENT_ID_LEN    24

typedef struct __component {
    char                id[COMPONENT_ID_LEN];
    int                 index;
    AREA                area;
    RGB                 fg;
    RGB                 bg;
    unsigned char       flags;
    struct ncplane      *plane;
    char                err_msg[ERR_MSG_LEN];
#define CMP_F_ENABLED   0x01
#define CMP_F_VISIBLE   0x02
} COMPONENT;

COMPONENT   component_new       (const char *, int, AREA *, RGB *, RGB *, unsigned char);
char        *component_init     (struct notcurses *, COMPONENT *);

#endif
