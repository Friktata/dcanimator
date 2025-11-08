/******************************************************************************
 *  dcanimator/include/dcanimator/main_menu.h
 *
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../core/app.h"

#include "component.h"
#include "ui.h"

char        *main_menu_init     (APP *, UI_COMPONENTS *);

#endif
