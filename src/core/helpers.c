/******************************************************************************
 *  dcanimator/src/core/list.c
 *
 */

#define _GNU_SOURCE

#include <wchar.h>
#include <notcurses/notcurses.h>

#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <strings.h>
#include <stdbool.h>

#include "../include/core/app.h"
#include "../include/core/config.h"
#include "../include/core/helpers.h"


/******************************************************************************
 *  RGB_get()
 *
 */
RGB RGB_get(
    CONFIG              *config,
    const char          *prefix
) {

    RGB                 rgb = {
        0, 0, 0
    };

    char                __red[1024];
    char                __green[1024];
    char                __blue[1024];

    memset(__red, '\0', 1024);
    memset(__green, '\0', 1024);
    memset(__blue, '\0', 1024);

    snprintf(__red, 1024, "%s_0", prefix);
    snprintf(__green, 1024, "%s_1", prefix);
    snprintf(__blue, 1024, "%s_2", prefix);

    char                *_red = config_get(config, __red);
    char                *_green = config_get(config, __green);
    char                *_blue = config_get(config, __blue);

    if (_red != NULL && (strcmp(_red, "(null)") != 0)) {
        rgb.red = atoi(_red);
    }
    if (_green != NULL && (strcmp(_green, "(null)") != 0)) {
        rgb.green = atoi(_green);
    }
    if (_blue != NULL && (strcmp(_blue, "(null)") != 0)) {
        rgb.blue = atoi(_blue);
    }

    return rgb;

}
