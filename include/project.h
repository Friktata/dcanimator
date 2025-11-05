/******************************************************************************
 *  dcanimator/include/project.h
 *
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <stdbool.h>

#include "app.h"
#include "path.h"
#include "config.h"

typedef struct __project {
    char                *path;
    char                *base;
    char                *name;
    int                 width;
    int                 height;
    CONFIG              settings;
    char                err_msg[ERR_MSG_LEN];
} PROJECT;

PROJECT     project_init        (CONFIG *);
bool        project_exists      (const char *);
char        *project_create     (const char *);

#endif
