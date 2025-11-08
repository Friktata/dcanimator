/******************************************************************************
 *  dcanimator/include/dcanimator/scenes.h
 *
 */

#ifndef SCENES_H
#define SCENES_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../core/app.h"

#include "component.h"
#include "ui.h"

char        *scenes_init  (APP *, UI_COMPONENTS *);

#endif
