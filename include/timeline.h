/******************************************************************************
 *  dcanimator/include/timeline.h
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "err.h"
#include "component.h"
#include "ui.h"

char        *timeline_init  (APP *, UI_COMPONENTS *);

#endif
