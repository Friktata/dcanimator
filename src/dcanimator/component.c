/******************************************************************************
 *  dcanimator/src/dcanimator/component.c
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
#include "../include/core/file.h"
#include "../include/core/list.h"
#include "../include/core/path.h"
#include "../include/core/config.h"
#include "../include/core/helpers.h"

#include "../include/dcanimator/component.h"

 COMPONENT component_new(
    const char          *id,
    int                 index,
    AREA                *area,
    RGB                 *fg,
    RGB                 *bg,
    unsigned char       flags
 ) {

    COMPONENT           component = {0};

    strncpy(component.id, id, (COMPONENT_ID_LEN - 1));
    
    component.index = index;

    memcpy(&component.area, area, sizeof(AREA));
    memcpy(&component.fg, fg, sizeof(RGB));
    memcpy(&component.bg, bg, sizeof(RGB));

    component.flags = flags;
    component.plane = NULL;
    
    memset(component.err_msg, '\0', ERR_MSG_LEN);

    return component;

 }

 char *component_init(
    struct notcurses    *nc,
    COMPONENT           *component
 ) {

    if (component->plane != NULL) {
        return NULL;
    };

    struct ncplane_options  opts = {
        .y = component->area.top,
        .x = component->area.left,
        .cols = component->area.width,
        .rows = component->area.height,
        .name = component->id,
        .resizecb = NULL,
        .flags = 0
    };

    if ((component->plane = ncplane_create(notcurses_stdplane(nc), &opts)) == NULL) {
        return app_seterror(component->err_msg, "Error in component_init(): %e\n");
    }

    ncplane_set_fg_rgb8(component->plane, component->fg.red, component->fg.green, component->fg.blue);
    ncplane_set_bg_rgb8(component->plane, component->bg.red, component->bg.green, component->bg.blue);

    uint64_t channels = 0;

    ncchannels_set_fg_rgb8(&channels, component->fg.red, component->fg.green, component->fg.blue);
    ncchannels_set_bg_rgb8(&channels, component->bg.red, component->bg.green, component->bg.blue);

    if(ncplane_set_base(component->plane, " ", 0, channels) < 0){
        return app_seterror(component->err_msg, "Error in project_start(): %e\n");
    }

    return NULL;

 }

