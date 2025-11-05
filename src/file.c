/******************************************************************************
 *  dcanimator/src/file.c
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#include <sys/stat.h>
#include <sys/types.h>

#include "../include/app.h"

#define FILE_BUF_SIZE   8192

/******************************************************************************
 *  file_load()
 *
 */
char *file_load(
    const char          *src,
    char                **dst,
    int                 max_bytes
) {

    FILE                *stream;
    long                file_size;

    static char         err_msg[ERR_MSG_LEN];

    memset(err_msg, '\0', ERR_MSG_LEN);

    if (src == NULL) {
        app_seterror(err_msg, "Error in load_file(): NULL parameter\n");
        goto __load_file_error;
    }

    if ((stream = fopen(src, "r")) == NULL) {
        app_seterror(err_msg, "Error in load_file(): Couldn\'t open file \'%s\' for reading (%e)\n", src);
        goto __load_file_error;
    }

    fseek(stream, 0, SEEK_END);
    file_size = ftell(stream);
    rewind(stream);

    if ((*dst = malloc(file_size + 1)) == NULL) {
        fclose(stream);
        app_seterror(err_msg, "Error in load_file(): %e\n");
        goto __load_file_error;
    }

    if (max_bytes > 0 && max_bytes < file_size) {
        file_size = max_bytes;
    }

    fread(*dst, 1, file_size, stream);
    (*dst)[file_size] = '\0';
    
    fclose(stream);

    return NULL;

__load_file_error:

    return err_msg;

}


/******************************************************************************
 *  file_save()
 *
 */
char *file_save(
    const char          *dst,
    const char          *src,
    int                 max_bytes
) {

    FILE                *stream;

    static char         err_msg[ERR_MSG_LEN];

    if (dst == NULL || src == NULL) {
        app_seterror(err_msg, "Error in save_file(): NULL parameter\n");
        goto __save_file_error;
    }

    if (*dst == '+') {
        if ((stream = fopen((dst + 1), "a")) == NULL) {
            app_seterror(err_msg, "Error in save_file(): Couldn\'t open file \'%s\' for appending (%e)\n", dst);
            goto __save_file_error;
        }
    }
    else {
        if ((stream = fopen(dst, "w")) == NULL) {
            app_seterror(err_msg, "Error in save_file(): Couldn\'t open file \'%s\' for appending (%e)\n", dst);
            goto __save_file_error;
        }
    }

    if (max_bytes <= 0) {
        max_bytes = strlen(src);
    }

    fwrite(src, 1, max_bytes, stream);
    fclose(stream);

    return NULL;

__save_file_error:

    return err_msg;

}


/******************************************************************************
 *  file_type()
 *
 */
char *file_type(
    const char          *path
) {

    struct stat         st;

    if (stat(path, &st) != 0) {
        return NULL;
    }

    if (S_ISREG(st.st_mode)) {
        return "file";
    }

    if (S_ISDIR(st.st_mode)) {
        return "directory";
    }

    if (st.st_mode & S_IXUSR) {
        return "executable";
    }

    return "other";

}


/******************************************************************************
 *  file_copy()
 *
 */
char *file_copy(
    const char          *src,
    const char          *dst
) {

    static char         err_msg[ERR_MSG_LEN];
    int                 in = open(src, O_RDONLY);

    memset(err_msg, '\0', ERR_MSG_LEN);

    if (in < 0) {
        app_seterror(err_msg, "Error in file_copy(): Source file \'%s\' not found (%e)\n", src);
        return err_msg;
    }

    struct stat st;

    fstat(in, &st);

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode);

    if (out < 0) {
        close(in);
        app_seterror(err_msg, "Error in file_copy(): Error opening destination file \'%s\' (%e)\n", src);
        return err_msg;
    }

    char buf[FILE_BUF_SIZE];
    ssize_t n;

    while ((n = read(in, buf, FILE_BUF_SIZE)) > 0) {
        if (write(out, buf, n) != n) {
            close(in);
            close(out);

            app_seterror(err_msg, "Error in file_copy(): (%e)\n");
            return err_msg;
        }
    }

    close(in);
    close(out);

    return NULL;
}
