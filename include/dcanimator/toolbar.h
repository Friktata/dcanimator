/******************************************************************************
 *  dcanimator/include/dcanimator/toolbar.h
 *
 */

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../core/app.h"

#include "component.h"
#include "ui.h"

char        *toolbar_init   (APP *, UI_COMPONENTS *);

#endif
