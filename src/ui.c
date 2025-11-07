/******************************************************************************
 *  dcanimator/src/ui.c
 *
 */

#define _GNU_SOURCE

#include <wchar.h>
#include <notcurses/notcurses.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "app.h"
#include "list.h"
#include "path.h"
#include "config.h"
#include "helpers.h"
#include "component.h"

#include "main_menu.h"

UI_COMPONENTS *ui_init(
    APP                     *app,
    struct notcurses        *nc
) {

    UI_COMPONENTS           ui_components = {
        .nc = nc,
        .term_x = app->term_x,
        .term_y = app->term_x,
    };

    memset(ui_components.err_msg, '\0', ERR_MSG_LEN);

    main_menu_init(app, &ui_components);

    if (app->err_msg[0] != '\0') {
        return NULL;
    }

    component_init(nc, &ui_components.component[UI_MAIN_MENU]);

    if (ui_components.component[UI_MAIN_MENU].err_msg[0] != '\0') {
        strncpy(app->err_msg, ui_components.component[UI_MAIN_MENU].err_msg, ERR_MSG_LEN);
        return NULL;
    }

    return &ui_components;

}
