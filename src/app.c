/******************************************************************************
 *  dcanimator/src/app.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../include/app.h"
#include "../include/file.h"
#include "../include/list.h"
#include "../include/config.h"
#include "../include/project.h"


/******************************************************************************
 *  app_new()
 *
 */
APP app_new() {
    
    APP                 app = {
        stdout,
        stderr,
        "",
        EXIT_SUCCESS,
        getenv("USER"),
        getenv("HOME"),
        config_new(),
        config_new()
    };

    return app;

}


/******************************************************************************
 *  app_seterror()
 *
 */
char *app_seterror(
    char                *err_buf,
    const char          *fmt,
    ...
) {

    va_list             args;

    char                *dst = err_buf;

    size_t              remaining = ERR_MSG_LEN;
    size_t              written;

    va_start(args, fmt);

    *dst = '\0';

    while (*fmt && remaining > 1) {

        if (*fmt == '%') {
            fmt++;

            if (*fmt == 'c') {
                written = snprintf(dst, remaining, "%c", (char)va_arg(args, int));
            }
            else if (*fmt == 'd') {
                written = snprintf(dst, remaining, "%d", va_arg(args, int));
            }
            else if (*fmt == 'f') {
                written = snprintf(dst, remaining, "%f", va_arg(args, double));
            }
            else if (*fmt == 's') {
                char *arg = va_arg(args, char *);

                written = snprintf(dst, remaining, "%s", arg ? arg : "(null)");
            }
            else if (*fmt == 'e') {
                written = snprintf(dst, remaining, "%s", strerror(errno));
            }
            else if (*fmt == '%') {
                written = snprintf(dst, remaining, "%%");
            }
            else {
                written = snprintf(dst, remaining, "%%%c", *fmt);
            }

            if (written >= remaining) {
                break;
            }

            dst += written;
            remaining -= written;
            fmt++;

            continue;
        }

        *dst++ = *fmt++;
        remaining--;
    }

    // app->exit_status = exit_status;

    *dst = '\0';
    va_end(args);

    return err_buf;

}


/******************************************************************************
 *  app_iserror()
 *
 */
bool app_iserror(
    APP                 *app
) {

    if (app->exit_status != EXIT_SUCCESS || app->err_msg[0] != '\0') {
        return true;
    }

    return false;

}


/******************************************************************************
 *  app_log()
 *
 */
void app_log(
    APP                 *app,
    const char          *fmt,
    ...
) {

    va_list             l;

    if (app->log_stream == NULL) {
        return;
    }

    va_start(l, fmt);
    vfprintf(app->log_stream, fmt, l);
    va_end(l);

    fflush(app->log_stream);

}


/******************************************************************************
 *  app_cleanup()
 *
 */
void app_cleanup(
    APP                 *app
) {

    config_free(&app->config);
    config_free(&app->args);

    if (app->log_stream != NULL) {
        if (app->log_stream != stdout && app->log_stream != stderr) {
            fclose(app->log_stream);
            app->log_stream = NULL;
        }
    }

    if (app->err_stream != NULL) {
        if (app->err_stream != stdout && app->err_stream != stderr) {
            fclose(app->err_stream);
            app->err_stream = NULL;
        }
    }

}


/******************************************************************************
 *  app_init()
 *
 */
void app_init(
    APP                 *app
) {

    PROJECT             project = project_init(&app->args);

    char                settings_path[PATHLEN_MAX];
    char                *err;

    memset(settings_path, '\0', PATHLEN_MAX);

    if (project_exists(project.path) == false) {
        app_log(app, "Creating new project \'%s\' in \'%s\'...\n", project.name, project.path);

        if ((err = project_create(project.name, project.path, project.width, project.height)) != NULL) {
            strncpy(app->err_msg, err, ERR_MSG_LEN);
            return;
        }

        app_log(app, "Created project %s successfully!\n", project.name);
    }
    else {
        app_log(app, "Opening existing project \'%s\'...\n", project.name);
    }

    strncpy(settings_path, path_next(project.path, ".settings"), PATHLEN_MAX);

    app_log(app, "Loading settings file \'%s\' for project \'%s\'...\n", settings_path, project.name);
    
    config_load(&project.settings, settings_path);

    if (project.err_msg[0] != '\0') {
        strncpy(app->err_msg, project.err_msg, ERR_MSG_LEN);
        return;
    }

    config_dump(&project.settings, app->log_stream, "Project settings");

    project_start(app, &project);

    config_free(&project.settings);

}
