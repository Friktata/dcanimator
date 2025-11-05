/******************************************************************************
 *  dcanimator/include/file.h
 *
 */

#ifndef FILE_H
#define FILE_H

#include "err.h"

char    *file_load      (const char *, char **, int);
char    *file_save      (const char *, const char *, int);
char    *file_type      (const char *);
char    *file_copy      (const char *, const char *);

#endif
