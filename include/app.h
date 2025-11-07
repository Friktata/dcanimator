/******************************************************************************
 *  dcanimator/include/app.h
 *
 */

#ifndef APP_H
#define APP_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define ERR_MSG_LEN     4096

#include "list.h"
#include "path.h"
#include "config.h"

#define UNAME_MAX_LEN   128

#ifndef PATHLEN_MAX
#define PATHLEN_MAX     512
#endif

#define INSTALL_PATH    "/usr/local/bin"
#define SHARE_PATH      "/usr/share/dcanimator"

typedef struct __app {
    FILE                *log_stream;
    FILE                *err_stream;
    char                err_msg[ERR_MSG_LEN];
    int                 exit_status;
    const char          *username;
    const char          *homepath;
    CONFIG              config;
    CONFIG              args;
    unsigned int        term_y;
    unsigned int        term_x;
} APP;

APP         app_new         (void);
char        *app_seterror   (char *, const char *, ...);
bool        app_iserror     (APP *);
void        app_log         (APP *, const char *, ...);
void        app_cleanup     (APP *);

void        app_init        (APP *);

#endif
