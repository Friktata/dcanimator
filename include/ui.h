/******************************************************************************
 *  dcanimator/include/ui.h
 *
 */

#define _GNU_SOURCE

#include <wchar.h>
#include <notcurses/notcurses.h>

#ifndef UI_H
#define UI_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define ERR_MSG_LEN     4096

#include "app.h"
#include "component.h"

typedef enum __components {
    UI_MAIN_MENU = 0,
    UI_COMPONENT_MAX
} COMPONENTS;

typedef struct __ui_components {
    struct notcurses        *nc;
    unsigned int            term_x;
    unsigned int            term_y;
    COMPONENT               component[UI_COMPONENT_MAX];
    char                    err_msg[ERR_MSG_LEN];
} UI_COMPONENTS;

UI_COMPONENTS   *ui_init    (APP *, struct notcurses *);

#endif
