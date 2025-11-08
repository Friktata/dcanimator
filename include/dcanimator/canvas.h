/******************************************************************************
 *  dcanimator/include//dcanimator/canvas.h
 *
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../core/app.h"

#include "component.h"
#include "ui.h"

char        *canvas_init    (APP *, UI_COMPONENTS *);

#endif
