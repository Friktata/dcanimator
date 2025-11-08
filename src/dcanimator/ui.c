/******************************************************************************
 *  dcanimator/src/dcanimator/ui.c
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

#include "../../include/core/app.h"
#include "../../include/core/list.h"
#include "../../include/core/path.h"
#include "../../include/core/config.h"
#include "../../include/core/helpers.h"

#include "../../include/dcanimator/component.h"
#include "../../include/dcanimator/main_menu.h"
#include "../../include/dcanimator/toolbar.h"
#include "../../include/dcanimator/timeline.h"
#include "../../include/dcanimator/layers.h"
#include "../../include/dcanimator/canvas.h"
#include "../../include/dcanimator/scenes.h"

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
    canvas_init(app, &ui_components);
    scenes_init(app, &ui_components);

    if (app->err_msg[0] != '\0') {
        return NULL;
    }

    component_init(nc, &ui_components.component[UI_MAIN_MENU]);
    component_init(nc, &ui_components.component[UI_TOOLBAR]);
    component_init(nc, &ui_components.component[UI_TIMELINE]);
    component_init(nc, &ui_components.component[UI_LAYERS]);
    component_init(nc, &ui_components.component[UI_CANVAS]);
    component_init(nc, &ui_components.component[UI_SCENES]);

    if (ui_components.component[UI_MAIN_MENU].err_msg[0] != '\0') {
        strncpy(app->err_msg, ui_components.component[UI_MAIN_MENU].err_msg, ERR_MSG_LEN);
        return NULL;
    }

    return &ui_components;

}
