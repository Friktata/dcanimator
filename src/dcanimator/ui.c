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

    static  UI_COMPONENTS   ui_components;

    ui_components.nc = nc;
    ui_components.term_x = app->term_x;
    ui_components.term_y = app->term_y;
    ui_components.mode = UI_MODE_SCENE;

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

    ui_components.component[UI_MAIN_MENU].f_view = NULL;
    ui_components.component[UI_TOOLBAR].f_view = NULL;
    ui_components.component[UI_TIMELINE].f_view = NULL;
    ui_components.component[UI_LAYERS].f_view = NULL;
    ui_components.component[UI_CANVAS].f_view = NULL;
    ui_components.component[UI_SCENES].f_view = &scenes_create_ui;

    ui_components.component[UI_MAIN_MENU].f_render = &component_init;
    ui_components.component[UI_TOOLBAR].f_render = &component_init;
    ui_components.component[UI_TIMELINE].f_render = &component_init;
    ui_components.component[UI_LAYERS].f_render = &component_init;
    ui_components.component[UI_CANVAS].f_render = &component_init;
    ui_components.component[UI_SCENES].f_render = &component_init;

    if (ui_components.component[UI_MAIN_MENU].err_msg[0] != '\0') {
        strncpy(app->err_msg, ui_components.component[UI_MAIN_MENU].err_msg, ERR_MSG_LEN);
        return NULL;
    }

    return &ui_components;

}

void ui_refresh(
    UI_COMPONENTS           *ui_components
) {

    for (int index = 0; index < UI_COMPONENT_MAX; index++) {
        
        if (ui_components->component[index].plane != NULL) {
            ncplane_destroy(ui_components->component[index].plane);
            ui_components->component[index].plane = NULL;
        }

        if (! (ui_components->component[index].flags & CMP_F_VISIBLE)) {
            if (! (ui_components->component[index].flags & CMP_F_REFRESH)) {
                continue;
            }
            else {
                ui_components->component[index].flags &= ~CMP_F_REFRESH;
            }
        }

        ui_components->component[index].f_render(ui_components->nc, &ui_components->component[index]);

        if (ui_components->component[index].f_view != NULL) {
            ui_components->component[index].f_view(&ui_components->component[index]);
        }

    }

}
