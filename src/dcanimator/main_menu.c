/******************************************************************************
 *  dcanimator/src/dcanimator/main_menu.c
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

#include "../include/core/app.h"
#include "../include/core/config.h"
#include "../include/core/helpers.h"

#include "../include/dcanimator/component.h"
#include "../include/dcanimator/ui.h"


/******************************************************************************
 *  main_menu_init()
 *
 */
char *main_menu_init(
    APP                 *app,
    UI_COMPONENTS       *ui_components
) {

    char    *_fg_red = config_get(&app->config, "main_menu_fg_0");
    char    *_fg_green = config_get(&app->config, "main_menu_fg_1");
    char    *_fg_blue = config_get(&app->config, "main_menu_fg_2");

    char    *_bg_red = config_get(&app->config, "main_menu_bg_0");
    char    *_bg_green = config_get(&app->config, "main_menu_bg_1");
    char    *_bg_blue = config_get(&app->config, "main_menu_bg_2");

    AREA    _area = {
        .top = 0,
        .left = 0,
        .width = ui_components->term_x,
        .height = 1
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

    ui_components->component[UI_MAIN_MENU] = component_new("Main Menu", UI_MAIN_MENU, &_area, &_fg, &_bg, (CMP_F_VISIBLE));

    return NULL;

}
