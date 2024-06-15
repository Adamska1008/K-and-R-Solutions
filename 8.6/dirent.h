#ifndef DIRENT_H
#define DIRENT_H
#define MAX_PATH 1024

#include <sys/types.h>

typedef struct
{
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[256];
} linux_dirent;

typedef struct
{
    int fd;
    linux_dirent d;
} Dir;

Dir *open_dir(char *dirname);
linux_dirent *read_dir(Dir *dfd);
void close_dir(Dir *dfd);

void fsize(char *);

#endif