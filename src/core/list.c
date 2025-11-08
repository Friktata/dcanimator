/******************************************************************************
 *  dcanimator/src/core/list.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <strings.h>
#include <stdbool.h>

#include "../include/core/app.h"


/******************************************************************************
 *  list_new()
 *
 */
LIST list_new(
    int                 max_items,
    int                 max_item_size,
    unsigned char       flags
) {

    LIST                list = {
        NULL,
        0,
        max_items,
        max_item_size,
        flags,
        ""
    };

    return list;

}


/******************************************************************************
 *  list_find()
 *
 */
int list_find(
    LIST                *list,
    const char          *item
) {

    for (int index = 0; index < list->items; index++) {
        if (list->flags & LIST_F_IGCASE) {
            if (strcasecmp(list->item[index], item) == 0) {
                return index;
            }
        }
        else {
            if (strcmp(list->item[index], item) == 0) {
                return index;
            }
        }
    }

    return -1;

}


/******************************************************************************
 *  list_validate()
 *
 */
char *list_validate(
    LIST                *list,
    const char          *item,
    char                **validated,
    bool                is_update
) {

    if (list == NULL || item == NULL || validated == NULL) {
        if (list == NULL) {
            return "Error in list_validate(): NULL parameter\n";
        }
        else {
            return app_seterror(list->err_msg, "Error in list_validate(): NULL parameter\n");
        }
    }

    if ((*validated = malloc(strlen(item) + 1)) == NULL) {
        return app_seterror(list->err_msg, "Error in list_validate(): \n");
    }

    memset(*validated, '\0', (strlen(item) + 1));
    strncpy(*validated, (char *) item, (strlen(item) + 1));

    if (is_update != true && list->max_items > 0) {
        if (list->items >= list->max_items) {
            fflush(stdout);
            app_seterror(list->err_msg, "Error in list_validate(): Cannot add item \'%s\' - exceeds max_items(%d)\n", (char *) item, list->max_items);
            goto __list_validate_error;
        }
    }

    if (list->max_item_size > 0) {
        if ((int) strlen(item) > list->max_item_size) {
            if (list->flags & LIST_F_NOCLIP) {
                app_seterror(list->err_msg, "Error in list_validate(): Cannot clip item \'%s\' - exceeds max_item_size (%d)\n", item, list->max_item_size);
                goto __list_validate_error;
            }

            (*validated)[list->max_item_size] = '\0';
        }
    }

    if (list->flags & LIST_F_UNIQUE) {
        if (list_find(list, *validated) >= 0) {
            app_seterror(list->err_msg, "Error in list_validate(): Cannot add dusplicate item \'%s\'\n", item);
            goto __list_validate_error;
        }
    }

    return NULL;


__list_validate_error:

    if (*validated != NULL) {
        free(*validated);
        *validated = NULL;
    }

    return list->err_msg;

}


/******************************************************************************
 *  list_add()
 *
 */
char *list_add(
    LIST                *list,
    const char          *item
) {

    char                **new_list = NULL;
    char                *validated = NULL;

    char                *return_status = NULL;

    int                 item_index;

    list->err_msg[0] = '\0';

    if (list == NULL || item == NULL) {
        return "Error in list_add(): NULL parameter\n";
    }

    if ((return_status = list_validate(list, item, &validated, false)) != NULL) {
        return return_status;
    }

    if (list->item == NULL) {
        item_index = 0;

        if ((new_list = malloc(sizeof(char *))) == NULL) {
            app_seterror(list->err_msg, "Error in list_add(): %e\n");
            goto __list_add_error;
        }

        list->items = 1;
    }
    else {
        item_index = list->items;

        char **tmp = realloc(list->item, sizeof(char *) * (list->items + 1));
        if (tmp == NULL) {
            app_seterror(list->err_msg, "Error in list_add(): %e\n");
            goto __list_add_error;
        }

        new_list = tmp;
        list->items++;
    }

    if ((new_list[item_index] = malloc(strlen(validated) + 1)) == NULL) {
        app_seterror(list->err_msg, "Error in list_add(): %e\n");
        goto __list_add_error;
    }

    strncpy(new_list[item_index], validated, (strlen(validated) + 1));

    list->item = new_list;

    free(validated);

    return NULL;
    

__list_add_error:

    if (list->err_msg[0] != '\0') {
        free(new_list);
        new_list = NULL;
    }

    if (validated != NULL) {
        for (int index = 0; index < list->items; index++) {
            if (list->item[index] != NULL) {
                free(list->item[index]);
                list->item[index] = NULL;
            }
        }

        free(validated);
        validated = NULL;
    }

    return list->err_msg;

}


/******************************************************************************
 *  list_delete()
 *
 */
char *list_delete(
    LIST                *list,
    int                 item_no
) {

    char                **new_list = NULL;

    if (list == NULL) {
        return "Error in list_delete(): NULL parameter\n";
    }

    if (item_no < 0 || item_no >= list->items) {
        return app_seterror(list->err_msg, "Error in list_delete(): Item index %d is out of range (0-%d)\n", item_no, (list->items - 1));
    }

    if (item_no == 0 && list->items == 1) {
        list_free(list);
        return NULL;
    }

    if (list->item[item_no] != NULL) {
        free(list->item[item_no]);
        list->item[item_no] = NULL;
    }

    for (int index = (item_no + 1); index < list->items; index++) {
        list->item[(index - 1)] = list->item[index];
    }

    if ((new_list = realloc(list->item, (sizeof(char *) * (--list->items)))) == NULL) {
        return app_seterror(list->err_msg, "Error in list_delete(): %e\n");
    }

    list->item = new_list;

    return NULL;

}


/******************************************************************************
 *  list_update()
 *
 */
char *list_update(
    LIST                *list,
    int                 item_no,
    const char          *item
) {

    char                *validated = NULL;
    char                *return_status = NULL;

    if (list == NULL || item == NULL) {
        return "Error in list_update(): NULL parameter\n";
    }

    if (item_no < 0 || item_no >= list->items) {
        return app_seterror(list->err_msg, "Error in list_update(): Item index %d is out of range (0-%d)\n", item_no, (list->items - 1));
    }
    
    if ((return_status = list_validate(list, item, &validated, true)) != NULL) {
        goto __list_update_cleanup;
    }
    
    if (list->item[item_no] != NULL) {
        free(list->item[item_no]);
        list->item[item_no] = NULL;
    }

    if ((list->item[item_no] = malloc(strlen(validated) + 1)) == NULL) {
        app_seterror(list->err_msg, "Error in list_update(): %e\n");
        goto __list_update_cleanup;
    }

    memset(list->item[item_no], '\0', (strlen(validated) + 1));
    strncpy(list->item[item_no], validated, (strlen(validated) + 1));


__list_update_cleanup:

    if (validated != NULL) {
        free(validated);
        validated = NULL;
    }

    return return_status;

}


/******************************************************************************
 *  list_dir()
 *
 */
char *list_dir(
    LIST                *list,
    const char          *dir_name
) {

    DIR                 *dir_handler;
    struct dirent       *dir_ent;

    char                *return_status = NULL;

    if (list == NULL || dir_name == NULL) {
        return "Error in list_dir(): NULL parameter\n";
    }

    if ((dir_handler = opendir(dir_name)) == NULL) {
        return app_seterror(list->err_msg, "Error in list_dir(): Error opening directory %s (%s)\n", dir_name);
    }

    while ((dir_ent = readdir(dir_handler))) {
        if ((return_status = list_add(list, dir_ent->d_name)) != NULL) {
            break;
        }
    }

    closedir(dir_handler);

    return return_status;

}


/******************************************************************************
 *  list_free()
 *
 */
void list_free(
    LIST                *list
) {

    if (list->item != NULL) {
        for (int index = 0; index < list->items; index++) {
            if (list->item[index] != NULL) {
                free(list->item[index]);
                list->item[index] = NULL;
            }
        }

        free(list->item);
        list->item = NULL;
    }

    list->items = 0;

}
