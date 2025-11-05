/******************************************************************************
 *  dcanimator/src/path.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/app.h"
#include "../include/list.h"
#include "../include/path.h"


/******************************************************************************
 *  path_name()
 *
 */
char *path_name(
    const char          *current
) {

    static char         path[PATHLEN_MAX];

    strncpy(path, current, PATHLEN_MAX);

    char                *src = &path[(strlen(current) - 1)];

    if (*src == PATH_SEPARATOR) {
        *src-- = '\0';
    }

    while (true) {
        if (src == &path[0] || *src == PATH_SEPARATOR) {
            if (*src == PATH_SEPARATOR) {
                src++;
            }    
            break;
        }

        src--;
    }

    return src;

}


/******************************************************************************
 *  path_nextg()
 *
 */
char *path_next(
    char                *base,
    char                *next
) {

    static char         path[PATHLEN_MAX];

    memset(path, '\0', PATHLEN_MAX);

    if ((base == NULL || base[0] == '\0') && (next == NULL || next[0] == '\0')) {
        path[0] = PATH_SEPARATOR;
        path[1] = '\0';

        return path;
    }

    if (base == NULL || base[0] == '\0') {
        return (char *) next;
    }

    if (base[(strlen(base) - 1)] == PATH_SEPARATOR) {
        base[(strlen(base) - 1)] = '\0';
    }

    if (next == NULL || next[0] == '\0') {
        return (char *) base;
    }

    if (base[0] == PATH_SEPARATOR && base[1] == '\0') {
        base++;
    }

    if (next[0] == PATH_SEPARATOR) {
        next++;
    }

    snprintf(path, PATHLEN_MAX, "%s%c%s", base, PATH_SEPARATOR, next);

    return path;

}


/******************************************************************************
 *  path_prev()
 *
 */
char *path_prev(
    char                *current
) {

    static char         path[PATHLEN_MAX];
    char                *path_ptr;

    memset(path, '\0', PATHLEN_MAX);

    if (current[0] == PATH_SEPARATOR && current[1] == '\0') {
        return (char *) current;
    }

    strncpy(path, current, PATHLEN_MAX);

    if (path[(strlen(path) - 1)] == PATH_SEPARATOR) {
        if (strlen(path) == 1) {
            return path;
        }

        path[(strlen(path) - 1)] = '\0';
    }

    for (path_ptr = &path[(strlen(path) - 1)]; ; path_ptr--) {
        char ch = *path_ptr;

        if (ch == PATH_SEPARATOR && path_ptr == path) {
            return path;
        }

        *path_ptr = '\0';

        if (ch == PATH_SEPARATOR) {
            break;
        }

        if (path_ptr == &path[0]) {
            break;
        }
    }

    return path;

}
