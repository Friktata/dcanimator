/******************************************************************************
 *  dcanimator/include/core/config.h
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifndef PATHLEN_MAX
#define PATHLEN_MAX     512
#endif

#include "app.h"

typedef struct __config {
    char                config_path[PATHLEN_MAX];
    char                err_msg[ERR_MSG_LEN];
    LIST                name;
    LIST                data;
} CONFIG;

CONFIG      config_new      (void);
int         config_find     (CONFIG *, const char *);
char        *config_set     (CONFIG *, const char *, const char *);
char        *config_get     (CONFIG *, const char *);
char        *config_load    (CONFIG *, const char *);
void        config_dump     (CONFIG *, FILE *, const char *);
char        *config_create  (const char *);
void        config_free     (CONFIG *);

#endif
