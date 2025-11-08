/******************************************************************************
 *  dcanimator/include/core/path.h
 *
 */

#ifndef PATH_H
#define PATH_H

#ifndef PATHLEN_MAX
#define PATHLEN_MAX     512
#endif

#ifdef _WIN32
#define PATH_SEPARATOR  '\\'
#else
#define PATH_SEPARATOR  '/'
#endif

char        *path_name      (const char *);
char        *path_next      (char *, char *);
char        *path_prev      (char *);

#endif
