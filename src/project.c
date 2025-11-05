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
    const char          *project_path
) {

    char                src[PATHLEN_MAX];
    char                dst[PATHLEN_MAX];

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

    if ((err = file_copy(src, dst)) != NULL) {
        return err;
    }

    memset(src, '\0', PATHLEN_MAX);
    strncpy(src, path_next((char *) project_path, "frames"), PATHLEN_MAX);

    if (mkdir(src, 0755) != 0) {
        app_seterror(err_msg, "£rror in project_create(): %e\n");
        return err_msg;
    }

    return NULL;

}
