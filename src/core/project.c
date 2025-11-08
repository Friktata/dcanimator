/******************************************************************************
 *  dcanimator/src/core/project.c
 *
 */
#define _GNU_SOURCE

#include <wchar.h>
#include <notcurses/notcurses.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../include/core/app.h"
#include "../include/core/file.h"
#include "../include/core/path.h"
#include "../include/core/config.h"
#include "../include/core/project.h"
#include "../include/core/helpers.h"

#include "../include/dcanimator/ui.h"


#define SETTINGS_OPTIONS    8


const char *__settings_option[SETTINGS_OPTIONS] = {
    "path",
    "name",
    "width",
    "height",
    "frames_per_second",
    "max_frames",
    "layers_per_frame",
    "undo_stack_size"
};


const char *__settings_template[SETTINGS_OPTIONS] = {
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Project path.\n\
//\n\
        path                \"%s\"\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Project name.\n\
//\n\
        name                \"%s\"\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Canvas width.\n\
//\n\
        width               %s\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Canvas height.\n\
//\n\
        height              %s\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Frames per second.\n\
//\n\
//  Can be either 12, 24, 30 or 60. The default is 24.\n\
//\n\
        frames_per_second   %s\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Max frames.\n\
//\n\
//  Setting this to 0 (default) places no limit on the total number of frames\n\
//  available in the project.\n\
//\n\
        max_frames          %s\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Layers per frame.\n\
//\n\
//  Any value in the inclusive range 1-12 is acceptable, the default setting\n\
//  is 6.\n\
//\n\
        layers_per_frame    %s\n\n\n\
",
    "///////////////////////////////////////////////////////////////////////////////\n\
//  Undo stack size.\n\
//\n\
//  Total number of undo/redo buffers in the stack - default value is 50\n\
//\n\
        undo_stack_size     %s\n\n\n\
",
};


/******************************************************************************
 *  project_init()
 *
 */
PROJECT project_init(
    CONFIG              *config
) {

    PROJECT             project = {
        config_get(config, "project_path"),
        config_get(config, "project_base"),
        config_get(config, "project_name"),
        atoi(config_get(config, "project_width")),
        atoi(config_get(config, "project_height")),
        config_new(),
        ""
    };

    return project;

}


/******************************************************************************
 *  project_config()
 *
 */
CONFIG project_config(
    const char          *project_name,
    const char          *project_path,
    int                 project_width,
    int                 project_height
) {

    CONFIG              config = config_new();

    char                width[20];
    char                height[20];

    snprintf(width, 20, "%d", project_width);
    snprintf(height, 20, "%d", project_height);

    config_set(&config, "path", project_path);
    config_set(&config, "name", project_name);

    config_set(&config, "width", width);
    config_set(&config, "height", height);

    config_set(&config, "frames_per_second", "24");
    config_set(&config, "max_frames", "0");

    config_set(&config, "layers_per_frame", "6");
    config_set(&config, "undo_stack_size", "50");

    return config;

}


/******************************************************************************
 *  project_save()
 *
 */
char *project_save(
    const char          *dst,
    CONFIG              *config
) {

    FILE                *stream = fopen(dst, "w");

    static char         err_msg[ERR_MSG_LEN];

    memset(err_msg, '\0', ERR_MSG_LEN);

    if (stream == NULL) {
        app_seterror(err_msg, "Error in project_save(): Couldn\n't open file \'%s\' for writing (%e)\n", dst);
        return err_msg;
    }

    fprintf(stream, "///////////////////////////////////////////////////////////////////////////////\n\
//  %s/.settings\n\
//\n\n\n", config_get(config, "path"));

    for (int index = 0; index < config->name.items; index++) {
        // if (strcmp(config->name.item[index], __settings_option[index])) {
        int option;

        for (option = 0; option < SETTINGS_OPTIONS; option++) {
            if (strcasecmp(__settings_option[option], config->name.item[index]) == 0) {
                break;
            }
        }

        if (option >= SETTINGS_OPTIONS) {
            app_seterror(err_msg, "Error in project_save(): Unknown option '\%s\'\n", config->name.item[index]);
        }

        fprintf(stream, __settings_template[option], config->data.item[index]);
    }

    fclose(stream);

    return NULL;

}


/******************************************************************************
 *  project_exists()
 *
 */
bool project_exists(
    const char          *project_path
) {

    char                *file_t = file_type(project_path);
    char                project_config[PATHLEN_MAX];

    memset(project_config, '\0', PATHLEN_MAX);

    if (file_t == NULL) {
        return false;
    }

    if (strcmp(file_t, "directory") != 0) {
        return false;
    }

    strncpy(project_config, path_next((char *) project_path, ".settings"), PATHLEN_MAX);

    file_t = file_type(project_config);

    if (file_t == NULL) {
        return false;
    }

    return true;

}


/******************************************************************************
 *  project_create()
 *
 */
char *project_create(
    const char          *project_name,
    const char          *project_path,
    int                 project_width,
    int                 project_height
) {

    char                dst[PATHLEN_MAX];
    char                *err = NULL;

    static char         err_msg[ERR_MSG_LEN];

    CONFIG              config = project_config(
        project_name,
        project_path,
        project_width,
        project_height
    );

    memset(dst, '\0', PATHLEN_MAX);

    if (mkdir(project_path, 0755) != 0) {
        app_seterror(err_msg, "£rror in project_create(): %e\n");
        return err_msg;
    }

    strncpy(dst, path_next((char *) project_path, ".settings"), PATHLEN_MAX);

    if ((err = project_save(dst, &config)) != NULL) {
        strncpy(err_msg, err, ERR_MSG_LEN);
        return err_msg;
    }

    config_free(&config);

    return NULL;

}


/******************************************************************************
 *  project_start()
 *
 */
char *project_start(
    APP                 *app,
    PROJECT             *project
) {

    struct notcurses_options opts = {0};

    struct notcurses    *nc = notcurses_init(NULL, NULL);
    struct ncplane      *display = notcurses_stddim_yx(nc, &app->term_y, &app->term_x);

    if (nc == NULL) {
        return app_seterror(app->err_msg, "Error in project_start(): Couldn\'t initialise notcurses...\n");
    }

    if (app->term_y < 34 || app->term_x < 94) {
        notcurses_stop(nc);
        return app_seterror(app->err_msg, "Error in project_start(): We need at least 34 rows and 94 columns - current dimensions are %d rows, %d columns\n", app->term_y, app->term_x);
    }

    notcurses_mice_enable(nc, NCMICE_ALL_EVENTS);
    
    RGB                 display_fg = RGB_get(&app->config, "display_fg");
    RGB                 display_bg = RGB_get(&app->config, "display_bg");

    ncplane_set_fg_rgb8(display, display_fg.red, display_fg.green, display_fg.blue);
    ncplane_set_bg_rgb8(display, display_bg.red, display_bg.green, display_bg.blue);

    uint64_t channels = 0;

    ncchannels_set_fg_rgb8(&channels, display_fg.red, display_fg.green, display_fg.blue);
    ncchannels_set_bg_rgb8(&channels, display_bg.red, display_bg.green, display_bg.blue);

    if(ncplane_set_base(display, " ", 0, channels) < 0){
        return app_seterror(app->err_msg, "Error in project_start(): %e\n");
    }

    UI_COMPONENTS       *components = ui_init(app, nc);

    if (app->err_msg[0] != '\0') {
        return NULL;
    }
    
    ncplane_erase(display);
    notcurses_render(nc);

    while (true) {
        ncplane_erase(display);
        ui_refresh(components);
        notcurses_render(nc);

        struct ncinput ni;
        int ch = notcurses_get_blocking(nc, &ni);

        // if(ni.evtype == NCTYPE_PRESS && ni.id == NCKEY_BUTTON1) {
        //     printf("Left click at %d,%d\n", ni.y, ni.x);
        // }

        if (ni.evtype == NCTYPE_PRESS && ni.id == NCKEY_BUTTON1) {
            if (ni.y == 4 && (ni.x >= 2 && ni.x < 11)) {
                fprintf(stdout, "New Scene");
                fflush(stdout);
            }
        }

        if (ch == 'q') {
            break;
        }
    }

__project_start_cleanup:

    notcurses_stop(nc);

    return NULL;

}
