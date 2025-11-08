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

#include "../include/app.h"
#include "../include/list.h"
#include "../include/path.h"
#include "../include/config.h"
#include "../include/helpers.h"
#include "../include/component.h"

#include "../include/main_menu.h"
#include "../include/toolbar.h"
#include "../include/timeline.h"
#include "../include/layers.h"

UI_COMPONENTS *ui_init(
    APP                     *app,
    struct notcurses        *nc
) {

    UI_COMPONENTS           ui_components = {
        .nc = nc,
        .term_x = app->term_x,
        .term_y = app->term_y,
    };

    memset(ui_components.err_msg, '\0', ERR_MSG_LEN);

    main_menu_init(app, &ui_components);
    toolbar_init(app, &ui_components);
    timeline_init(app, &ui_components);
    layers_init(app, &ui_components);

    if (app->err_msg[0] != '\0') {
        return NULL;
    }

    component_init(nc, &ui_components.component[UI_MAIN_MENU]);
    component_init(nc, &ui_components.component[UI_TOOLBAR]);
    component_init(nc, &ui_components.component[UI_TIMELINE]);
    component_init(nc, &ui_components.component[UI_LAYERS]);

    if (ui_components.component[UI_MAIN_MENU].err_msg[0] != '\0') {
        strncpy(app->err_msg, ui_components.component[UI_MAIN_MENU].err_msg, ERR_MSG_LEN);
        return NULL;
    }

    return &ui_components;

}
