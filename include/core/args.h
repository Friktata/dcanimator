/******************************************************************************
 *  dcanimator/include/core/args.h
 *
 */

#ifndef ARGS_H
#define ARGS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "app.h"
#include "file.h"
#include "list.h"
#include "path.h"
#include "config.h"

void        args_process    (APP *, int, const char **);

#endif
