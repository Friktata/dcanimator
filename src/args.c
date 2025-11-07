/******************************************************************************
 *  dcanimator/src/args.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "../include/app.h"
#include "../include/file.h"
#include "../include/list.h"
#include "../include/path.h"
#include "../include/config.h"


/******************************************************************************
 *  __args_init()
 *
 */
void __args_init(
    APP                     *app
) {

    char                    path[PATHLEN_MAX];
    char                    base[PATHLEN_MAX];
    char                    project_path[PATHLEN_MAX];

    memset(path, '\0', PATHLEN_MAX);
    memset(base, '\0', PATHLEN_MAX);
    memset(project_path, '\0', PATHLEN_MAX);

    strncpy(path, app->homepath, PATHLEN_MAX);
    getcwd(base, PATHLEN_MAX);
    snprintf(project_path, PATHLEN_MAX, "%s%cNew_Project", base, PATH_SEPARATOR);

    strncpy(path, path_next(path, ".dcanimator"), PATHLEN_MAX);

    config_set(&app->args, "log_stream", "--stdout");
    config_set(&app->args, "err_stream", "--stderr");

    config_set(&app->args, "start_path", base);
    config_set(&app->args, "config_path", path);

    config_set(&app->args, "project_path", project_path);
    config_set(&app->args, "project_base", base);
    config_set(&app->args, "project_name", "New_Project");

    config_set(&app->args, "project_height", "24");
    config_set(&app->args, "project_width", "80");

}


/******************************************************************************
 *  args_process()
 *
 */
char *args_process(
    APP                     *app,
    int                     argc,
    const char              **argv
) {

    __args_init(app);

    for (int arg = 1; arg < argc; arg++) {

    //  --log
    //
    //  Specify a log/output file.
    //
        if (strcmp(argv[arg], "--log") == 0) {
            if (++arg >= argc) {
                app_seterror(app->err_msg, "Error in args_process(): The --log option requires a parameter\n");
                return NULL;
            }

            if (argv[arg][0] == '-' && argv[arg][0] == '-') {
                if ((strcmp(argv[arg], "--stdout") != 0) && (strcmp(argv[arg], "--stderr") != 0) && (strcmp(argv[arg], "--none") != 0)) {
                    app_seterror(app->err_msg, "Error in args_process(): \'%s\' is an invalid parameter for the --log option\n", argv[arg]);
                    return NULL;
                }
            }

            config_set(&app->args, "log_path", argv[arg]);

            if (app->log_stream != NULL) {
                if (app->log_stream != stdout && app->log_stream != stderr) {
                    fprintf(stdout, "CLOSING LOG STREAM\n");
                    fclose(app->log_stream);
                }
                app->log_stream = NULL;
            }

            if (argv[arg][0] != '-' || argv[arg][1] != '-') {
                if ((app->log_stream = fopen(argv[arg], "w")) == NULL) {
                    app_seterror(app->err_msg, "Error in args_process(): Error opening log file \'%s\' for writing (%e)\n", argv[arg]);
                }
            }
            else {
                if (strcmp(argv[arg], "--stdout") == 0) {
                    app->log_stream = stdout;
                }
                if (strcmp(argv[arg], "--stderr") == 0) {
                    app->log_stream = stderr;
                }
            }

            continue;
        }

    //  --err
    //
    //  Specify an error file.
    //
        if (strcmp(argv[arg], "--err") == 0) {
            if (++arg >= argc) {
                app_seterror(app->err_msg, "Error in args_process(): The --err option requires a parameter\n");
                return NULL;
            }

            if (argv[arg][0] == '-' && argv[arg][0] == '-') {
                if ((strcmp(argv[arg], "--stdout") != 0) && (strcmp(argv[arg], "--stderr") != 0)) {
                    app_seterror(app->err_msg, "Error in args_process(): \'%s\' is an invalid parameter for the --err option\n", argv[arg]);
                    return NULL;
                }
            }

            config_set(&app->args, "err_path", argv[arg]);

            if (app->err_stream != NULL) {
                if (app->err_stream != stdout && app->err_stream != stderr) {
                    fclose(app->err_stream);
                }
                app->err_stream = NULL;
            }

            if (argv[arg][0] != '-' && argv[arg][1] != '-') {
                if ((app->err_stream = fopen(argv[arg], "w")) == NULL) {
                    app_seterror(app->err_msg, "Error in args_process(): Error opening error file \'%s\' for writing (%e)\n", argv[arg]);
                }
            }
            else {
                if (strcmp(argv[arg], "--stdout") == 0) {
                    app->err_stream = stdout;
                }
                if (strcmp(argv[arg], "--stderr") == 0) {
                    app->err_stream = stderr;
                }
            }

            continue;
        }

    //  --config
    //
    //  Specify a config alternative to the default.
    //
        if (strcmp(argv[arg], "--config") == 0) {
            if (++arg >= argc || (argv[arg][0] == '-' && argv[arg][1] == '-')) {
                app_seterror(app->err_msg, "Error in args_process(): The --config option requires a parameter\n");
                return NULL;
            }

            config_set(&app->args, "config_path", argv[arg]);
            continue;
        }

    //  --project
    //
    //  Specify project path/name.
    //
        if (strcmp(argv[arg], "--project") == 0) {
            if (++arg >= argc || (argv[arg][0] == '-' && argv[arg][1] == '-')) {
                app_seterror(app->err_msg, "Error in args_process(): The --project option requires a parameter\n");
                return NULL;
            }

            char *base = path_prev((char *) argv[arg]);
            char *name = path_name((const char *) argv[arg]);

            config_set(&app->args, "project_path", argv[arg]);

            if (base[0] != '\0') {
                config_set(&app->args, "project_base", base);
            }    
            else {
                config_set(&app->args, "project_path", path_next(config_get(&app->args, "project_base"), name));
            }
    
            config_set(&app->args, "project_name", name);

            continue;
        }

    //  --height
    //
    //  Specify project height (rows/lines).
    //
        if (strcmp(argv[arg], "--height") == 0) {
            if (++arg >= argc || (argv[arg][0] == '-' && argv[arg][1] == '-')) {
                app_seterror(app->err_msg, "Error in args_process(): The --height option requires a parameter\n");
                return NULL;
            }

            config_set(&app->args, "project_height", argv[arg]);

            continue;
        }

    //  --width
    //
    //  Specify project width (characters/columns).
    //
        if (strcmp(argv[arg], "--width") == 0) {
            if (++arg >= argc || (argv[arg][0] == '-' && argv[arg][1] == '-')) {
                app_seterror(app->err_msg, "Error in args_process(): The --width option requires a parameter\n");
                return NULL;
            }

            config_set(&app->args, "project_width", argv[arg]);

            continue;
        }

    }

    return NULL;

}
