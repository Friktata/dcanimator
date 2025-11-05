/******************************************************************************
 *  dcanimator/src/project.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../include/app.h"
#include "../include/file.h"
#include "../include/path.h"
#include "../include/config.h"
#include "../include/project.h"


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

    char                src[PATHLEN_MAX];
    char                dst[PATHLEN_MAX];

    char                *file_data = NULL;
    FILE                *stream;

    static char         err_msg[ERR_MSG_LEN];

    char                *err = NULL;

    memset(src, '\0', PATHLEN_MAX);
    memset(dst, '\0', PATHLEN_MAX);

    if (mkdir(project_path, 0755) != 0) {
        app_seterror(err_msg, "£rror in project_create(): %e\n");
        return err_msg;
    }

    strncpy(src, path_next(SHARE_PATH, ".settings"), PATHLEN_MAX);
    strncpy(dst, path_next((char *) project_path, ".settings"), PATHLEN_MAX);

    // if ((err = file_copy(src, dst)) != NULL) {
    //     return err;
    // }

    if ((err = file_load(src, &file_data, 0)) != NULL) {
        strncpy(err_msg, err, ERR_MSG_LEN);
        return err_msg;
    }

    if ((stream = fopen(dst, "w")) == NULL) {
        free(file_data);
        app_seterror(err_msg, "Error in project_create(): %e\n");
        return err_msg;
    }

    fprintf(stream, "\
///////////////////////////////////////////////////////////////////////////////\n\
//  %s/.settings\n\
//\n\
\n\n\
///////////////////////////////////////////////////////////////////////////////\n\
//  Project name.\n\
//\n\
    name                %s\n\
\n\n\
///////////////////////////////////////////////////////////////////////////////\n\
//  Project height/width (rows/columns)\n\
//\n\
    height              %d\n\
    width               %d\n\
\n\n\
%s\n\
\n\
    ", project_path, project_name, project_width, project_height, file_data);

    fclose(stream);
    free(file_data);

    memset(src, '\0', PATHLEN_MAX);
    strncpy(src, path_next((char *) project_path, "frames"), PATHLEN_MAX);

    if (mkdir(src, 0755) != 0) {
        app_seterror(err_msg, "£rror in project_create(): %e\n");
        return err_msg;
    }

    return NULL;

}
