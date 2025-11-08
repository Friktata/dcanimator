/******************************************************************************
 *  dcanimator/include/layers.h
 *
 */

#ifndef LAYERS_H
#define LAYERS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "err.h"
#include "component.h"
#include "ui.h"

char        *layers_init    (APP *, UI_COMPONENTS *);

#endif
