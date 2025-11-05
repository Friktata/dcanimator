/******************************************************************************
 *  dcanimator/src/config.c
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
#include "../include/parse.h"


/******************************************************************************
 *  config_new()
 *
 */
CONFIG config_new() {

   CONFIG                  config = {
       "",
       "",
       list_new(0, 0, 0),
       list_new(0, 0, 0)
   };

   return config;

}


/******************************************************************************
 *  config_find()
 *
 */
int config_find(
    CONFIG                  *config,
    const char              *name
) {

    return list_find(&config->name, name);

}


/******************************************************************************
 *  config_set()
 *
 */
char *config_set(
    CONFIG                  *config,
    const char              *name,
    const char              *data
) {

    int                     index = config_find(config, name);

    if (index < 0) {
        if (list_add(&config->name, name) != NULL) {
            return config->name.err_msg;
        }
        if (list_add(&config->data, data) != NULL) {
            return config->data.err_msg;
        }
    }
    else {
        if (list_update(&config->data, index, data) != NULL) {
            return config->data.err_msg;
        }
    }

    return NULL;

}


/******************************************************************************
 *  config_get()
 *
 */
char *config_get(
    CONFIG                  *config,
    const char              *name
) {

    int                     index = config_find(config, name);

    if (index < 0) {
        return NULL;
    }
    
    // if (list_update(&config->data, index, data) != NULL) {
    //     return config->data.err_msg;
    // }
    
    return config->data.item[index];

}


/******************************************************************************
 *  config_load()
 *
 */
char *config_load(
    CONFIG                  *config,
    const char              *path
) {

    char                    *file_data = NULL;
    char                    *tmp = NULL;
    char                    *err = NULL;

    LIST                    lines = list_new(0, 0, 0);
    LIST                    tokens = list_new(0, 0, 0);

    if ((err = file_load(path, &file_data, 0)) != NULL) {
        strncpy(config->err_msg, err, ERR_MSG_LEN);
        goto __config_load_cleanup;
    }

    if ((tmp = malloc(strlen(file_data) + 1)) == NULL) {
        app_seterror(config->err_msg, "Error in config_load(): %e\n");
        goto __config_load_cleanup;
    }

    if ((err = parse_lines(&lines, file_data)) != NULL) {
        strncpy(config->err_msg, err, ERR_MSG_LEN);
        goto __config_load_cleanup;
    }

    for (int line = 0; line < lines.items; line++) {

        char *token = "";

        if ((err = parse_line(&tokens, lines.item[line])) != NULL) {
            strncpy(config->err_msg, err, ERR_MSG_LEN);
            goto __config_load_cleanup;
        }

        if (tokens.items > 2) {
            token = tokens.item[2];
        }
        
        if (tokens.items < 3) {
            if ((err = config_set(config, tokens.item[1], token)) != NULL) {
                strncpy(config->err_msg, err, ERR_MSG_LEN);
                goto __config_load_cleanup;
            }
        }
        else {
            for (int index = 2; index < tokens.items; index++) {
                snprintf(tmp, (strlen(file_data) + 1), "%s_%i", tokens.item[1], (index - 2));

                if ((err = config_set(config, tmp, tokens.item[index])) != NULL) {
                    strncpy(config->err_msg, err, ERR_MSG_LEN);
                    goto __config_load_cleanup;
                }
            }
        }

        list_free(&tokens);

    }


__config_load_cleanup:

    if (tmp != NULL) {
        free(tmp);
        tmp = NULL;
    }

    if (file_data != NULL) {
        free(file_data);
        file_data = NULL;
    }

    list_free(&lines);
    list_free(&tokens);

    if (config->err_msg[0] != '\0') {
        return config->err_msg;
    }

    return NULL;

}


/******************************************************************************
 *  config_check()
 *
 */
char *config_check(
    const char              *path
) {

}


/******************************************************************************
 *  config_dump()
 *
 */
void config_dump(
    CONFIG                  *config,
    FILE                    *stream,
    const char              *config_name
) {

    if (stream == NULL) {
        return;
    }

    fprintf(stream, "Dumping %s config (%d entries):\n\n", config_name, config->data.items);

    for (int index = 0; index < config->name.items; index++) {
        fprintf(stream, "\t%s=%s\n", config->name.item[index], config->data.item[index]);
    }

    fprintf(stream, "\n");

}


/******************************************************************************
 *  config_create()
 *
 */
char *config_create(
    const char              *path
) {

    char                    template_path[PATHLEN_MAX];

    memset(template_path, '\0', PATHLEN_MAX);
    strncpy(template_path, path_next(SHARE_PATH, "templates/.dcanimator"), PATHLEN_MAX);

    return file_copy(template_path, path);

}


/******************************************************************************
 *  config_free()
 *
 */
void config_free(
    CONFIG                  *config
) {

    list_free(&config->name);
    list_free(&config->data);

}
