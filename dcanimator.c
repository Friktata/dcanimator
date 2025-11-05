/******************************************************************************
 *  dcanimator/dcanimator.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/app.h"
#include "include/list.h"
#include "include/file.h"
#include "include/path.h"
#include "include/parse.h"
#include "include/config.h"

#include "include/args.h"

int main(
    int                 argc,
    char                **argv
) {

    APP                 app = app_new();

    args_process(&app, argc, (const char **) argv);

    if (app.err_msg[0] != '\0') {
        goto __main_error;
    }

    config_dump(&app.args, app.log_stream, "command-line");

    char                *config_path = config_get(&app.args, "config_path");
    char                *file_t = file_type((const char *) config_path);

    if (file_t == NULL) {
        app_log(&app, "Config %s not found...\n", config_path);

        char *err = config_create(config_path);

        if (err != NULL) {
            strncpy(app.err_msg, err, ERR_MSG_LEN);
            goto __main_error;
        }

        app_log(&app, "Generated new default config %s\n", config_path);
    }
    else {
        if (strcmp(file_t, "file") != 0) {
            app_seterror(app.err_msg, "Error: config_path \'%s\' is not a regular file (%s)\n", config_path, file_t);
            goto __main_error;
        }
    }

    app_log(&app, "Loading main config from %s...\n", config_path);

    config_load(&app.config, config_path);

    if (app.config.err_msg[0] != '\0') {
        strncpy(app.err_msg, app.config.err_msg, ERR_MSG_LEN);
        goto __main_error;
    }

    config_dump(&app.config, app.log_stream, "main app");

    app_log(&app, "Initialising dcanimator...\n");
    app_init(&app);

    if (app.err_msg[0] != '\0') {
        goto __main_error;
    }

    goto __main_cleanup;


__main_error:

    if (app.err_msg[0] != '\0') {
        fprintf(app.err_stream, "%s", app.err_msg);
    }


__main_cleanup:

    app_cleanup(&app);

    return app.exit_status;

}
