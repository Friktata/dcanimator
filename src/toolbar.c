/******************************************************************************
 *  dcanimator/src/toolbar.c
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
#include <unistd.h>

#include "../include/app.h"
#include "../include/config.h"
#include "../include/helpers.h"
#include "../include/component.h"
#include "../include/ui.h"


/******************************************************************************
 *  toolbar_init()
 *
 */
char *toolbar_init(
    APP                 *app,
    UI_COMPONENTS       *ui_components
) {

    char    *_fg_red = config_get(&app->config, "toolbar_fg_0");
    char    *_fg_green = config_get(&app->config, "toolbar_fg_1");
    char    *_fg_blue = config_get(&app->config, "toolbar_fg_2");

    char    *_bg_red = config_get(&app->config, "toolbar_bg_0");
    char    *_bg_green = config_get(&app->config, "toolbar_bg_1");
    char    *_bg_blue = config_get(&app->config, "toolbar_bg_2");

    AREA    _area = {
        .top = 2,
        .left = 1,
        .width = 5,
        .height = (ui_components->term_y - 11)
    };

    RGB     _fg = { 255, 255, 255 };
    RGB     _bg = { 255, 255, 255 };

    if (_fg_red != NULL && (strcmp(_fg_red, "(null)") != 0)) {
        _fg.red = (unsigned int) atoi(_fg_red);
    }
    if (_fg_green != NULL && (strcmp(_fg_green, "(null)") != 0)) {
        _fg.green = (unsigned int) atoi(_fg_green);
    }
    if (_fg_blue != NULL && (strcmp(_fg_blue, "(null)") != 0)) {
        _fg.blue = (unsigned int) atoi(_fg_blue);
    }

    if (_bg_red != NULL && (strcmp(_bg_red, "(null)") != 0)) {
        _bg.red = (unsigned int) atoi(_bg_red);
    }
    if (_bg_green != NULL && (strcmp(_bg_green, "(null)") != 0)) {
        _bg.green = (unsigned int) atoi(_bg_green);
    }
    if (_bg_blue != NULL && (strcmp(_bg_blue, "(null)") != 0)) {
        _bg.blue = (unsigned int) atoi(_bg_blue);
    }

    ui_components->component[UI_TOOLBAR] = component_new("Toolbar", 2, &_area, &_fg, &_bg, 0);

    return NULL;

}
