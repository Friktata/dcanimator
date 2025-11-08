/******************************************************************************
 *  dcanimator/include/core/list.h
 *
 */

#ifndef LIST_H
#define LIST_H

#include "app.h"

typedef struct __list {
    char                **item;
    int                 items;
    int                 max_items;
    int                 max_item_size;
    unsigned char       flags;
    char                err_msg[ERR_MSG_LEN];
#define LIST_F_UNIQUE   0x01
#define LIST_F_NOCLIP   0x02
#define LIST_F_IGCASE   0x04
#define LIST_F_IGWRNG   0x08
} LIST;

LIST    list_new        (int, int, unsigned char);
int     list_find       (LIST *, const char *);
char    *list_validate  (LIST *, const char *, char **, bool);
char    *list_add       (LIST *, const char *);
char    *list_delete    (LIST *, int);
char    *list_update    (LIST *, int, const char *);
char    *list_dir       (LIST *, const char *);
void    list_free       (LIST *);

#endif
