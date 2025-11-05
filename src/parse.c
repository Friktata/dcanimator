/******************************************************************************
 *  dcanimator/src/parse.c
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "app.h"
#include "list.h"


/******************************************************************************
 *  __is_quote_char()
 *
 */
bool __is_quote_char(
    char                ch
) {
    
    return (
        ch == '\''      ||
        ch == '\"'      ||
        ch == '`'
    ) ? true : false;

}


/******************************************************************************
 *  __is_whitespace_char()
 *
 */
bool __is_whitespace_char(
    char                ch
) {
    
    return (
        ch == ' '       ||
        ch == '\t'
    ) ? true : false;

}


/******************************************************************************
 *  __is_single_char()
 *
 */
bool __is_single_char(
    char                ch
) {
    
    return (
        ch == '='       ||
        ch == '/'       ||
        ch == '*'       ||
        ch == '-'       ||
        ch == '+'       ||
        ch == '['       ||
        ch == ']'       ||
        ch == '{'       ||
        ch == '}'       ||
        ch == '('       ||
        ch == ')'
    ) ? true : false;

}


/******************************************************************************
 *  __buf_is_empty()
 *
 */
bool __buf_is_empty(
    char                *buf
) {

    for (int index = 0; index < strlen(buf); index++) {
        if (! __is_whitespace_char(buf[index]) && buf[index] != '\n' && buf[index] != '\r') {
            return false;
        }
    }

    return true;

}


/******************************************************************************
 *  parse_lines()
 *
 */
char *parse_lines(
    LIST                *list,
    char                *data
) {

    char                *buf = NULL;
    char                *tmp = NULL;

    char                *src = NULL;
    char                *dst = NULL;

    char                quote_char = '\0';
    char                last_char = '\0';

    int                 current_line = 1;
    int                 start_line = 1;

    char                *return_status = NULL;
    static  char        err_msg[ERR_MSG_LEN];

    bool                is_comment = false;
    bool                block_comment = false;


    memset(err_msg, '\0', ERR_MSG_LEN);

    if (list == NULL || data == NULL) {
        app_seterror(err_msg, "Error in parse_lines(): NULL parameter\n");
        return err_msg;
    }

    buf = malloc(strlen(data) + 20);
    tmp = malloc(strlen(data) + 20);

    if (buf == NULL || tmp == NULL) {
        app_seterror(list->err_msg, "Error in parse_lines(): %e\n");
        return list->err_msg;
    }

    memset(tmp, '\0', (strlen(data) + 1));

    dst = tmp;
    src = data;

    while (true) {

        char            ch = *src++;

        if (ch == '\0') {
            break;
        }

        if ((last_char != '\\' && quote_char == '\0') && (ch == '/' && *src == '/')) {
            if (is_comment == false && block_comment == false) {
                is_comment = true;
            }
        }

        if ((last_char != '\\' && quote_char == '\0') && (ch == '/' && *src == '*')) {
            if (is_comment == false && block_comment == false) {
                block_comment = true;
            }
        }

        if ((last_char != '\\' && quote_char == '\0') && (ch == '*' && *src == '/')) {
            if (block_comment == true) {
                is_comment = block_comment = false;
                src++;
                continue;
            }
        }

        if (strlen(tmp) < 1) {
            start_line = current_line;
        }

        if (ch == '\n' || ch == '\r') {
            if (is_comment == false && block_comment == false) {
                *dst++ = ch;
            }
            current_line++;

            if (ch == '\r' && *src == '\n') {
                ch = *src++;
                *dst++ = ch;
            }

            if (quote_char == '\0' && last_char != '\\') {
                if (is_comment == true) {
                    is_comment = false;
                    continue;
                }

                if (strlen(tmp) > 0) {
                    if (__buf_is_empty(tmp) == false) {
                        snprintf(buf, (strlen(data) + 20), "%d %s", start_line, tmp);

                        if ((return_status = list_add(list, buf)) != NULL) {
                            goto __parse_lines_cleanup;
                        }
                    }

                    memset(buf, '\0', (strlen(data) + 1));
                    memset(tmp, '\0', (strlen(data) + 1));

                    dst = tmp;
                    
                    continue;
                }
            }
        }

        if (__is_quote_char(ch) && last_char != '\\') {
            if (quote_char == ch) {
                quote_char = '\0';
            }
            else {
                quote_char = ch;
            }
        }

        if (is_comment == false && block_comment == false) {
            *dst++ = ch;
            *dst = '\0';
        }

        last_char = ch;

    }

    if (quote_char != '\0') {
        app_seterror(err_msg, "Error on line %d: Unterminated quoted string\n", start_line);
    }

    if (strlen(tmp) > 0) {
        if (__buf_is_empty(tmp) == false) {
            snprintf(buf, (strlen(data) + 20), "%d %s", start_line, tmp);

            if ((return_status = list_add(list, buf)) != NULL) {
                goto __parse_lines_cleanup;
            }
        }
    }


__parse_lines_cleanup:

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    if (tmp != NULL) {
        free(tmp);
        tmp = NULL;
    }

    if (list->err_msg[0] != '\0') {
        return list->err_msg;
    }

    return NULL;

}


/******************************************************************************
 *  parse_line()
 *
 */
char *parse_line(
    LIST                *list,
    const char          *line
) {

    char                quote_char = '\0';
    char                last_char = '\0';

    int                 paren = 0;
    int                 curly = 0;
    int                 block = 0;

    char                *token = NULL;

    char                *src = (char *) line;
    char                *dst;

    static char         err_msg[ERR_MSG_LEN];

    memset(err_msg, '\0', ERR_MSG_LEN);

    if (list == NULL || line == NULL) {
        app_seterror(err_msg, "Error in parse_line(): NULL parameter\n");
        return err_msg;
    }

    if ((token = malloc(strlen(line) + 1)) == NULL) {
        app_seterror(list->err_msg, "Error in parse_line(): %e\n");
        goto __parse_line_cleanup;
    }

    memset(token, '\0', (strlen(line) + 1));
    dst = token;

    while (true) {

        char            ch = *src++;

        if (ch == '\0') {
            break;
        }
        
        if (__is_whitespace_char(ch) || ch == '\n' || ch == '\r') {
            continue;
        }

        if (__is_single_char(ch)) {
            token[0] = ch;
            token[1] = '\0';

            goto __add_token;
        }

        if (__is_quote_char(ch)) {
            quote_char = ch;

            *dst++ = ch;
            *dst = '\0';

            while (true) {
                char byte = *src++;

                if (byte == '\0') {
                    break;
                }

                if (byte == quote_char) {
                    fflush(stdout);
                    *dst++ = byte;
                    *dst = '\0';
                    break;
                }

                *dst++ = byte;
                *dst = '\0';
            }

            goto __add_token;
        }

        *dst++ = ch;
        *dst = '\0';

        while (*src != '\0') {
            char byte = *src++;

            if (
                last_char != '\\'               &&
                (
                    __is_whitespace_char(byte)  ||
                    __is_single_char(byte)      ||
                    __is_quote_char(byte)       ||
                    byte == '\n'                ||
                    byte == '\r'
                )
            ) {
                src--;
                break;
            }

            *dst++ = byte;
            *dst = '\0';

            last_char = byte;
        }

__add_token:

        if (__buf_is_empty(token) == false) {

            if (list_add(list, token) != NULL) {
                goto __parse_line_cleanup;
            }
        
        }

        memset(token, '\0', (strlen(line) + 1));
        dst = token;

    }


__parse_line_cleanup:

    if (token != NULL) {
        free(token);
        token = NULL;
    }

    if (list->err_msg[0] != '\0') {
        return list->err_msg;
    }

    return NULL;

}
