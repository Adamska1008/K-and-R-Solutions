#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "dirent.h"

int fstat(int fd, struct stat *);

Dir *opendir(char *dirname)
{
    int fd;
    struct stat stbuf;
    Dir *dp;

    if ((fd = open(dirname, O_RDONLY, 0)) == -1 || fstat(fd, &stbuf) == -1 || (stbuf.st_mode & __S_IFMT) != __S_IFDIR || (dp = (Dir *)malloc(sizeof(Dir))) == NULL)
        return NULL;
    dp->fd = fd;
    return dp;
}

linux_dirent *readdir(Dir *dp)
{
    static linux_dirent dirent;
    static unsigned long offset;
    static char *buf;
    static int nread;
    if (buf == NULL)
    {
        buf = (char *)malloc(BUFSIZ);
        nread = syscall(SYS_getdents, dp->fd, buf, BUFSIZ);
        if (nread == -1)
        {
            perror("syscall(SYS_getdents)");
            exit(EXIT_FAILURE);
        }
        if (nread == 0)
            return NULL;
    }
    if (offset >= nread)
        return NULL;
    dirent = *(linux_dirent *)(buf + offset);
    offset += dirent.d_reclen;
    return &dirent;
}

void closedir(Dir *dp)
{
    if (dp)
    {
        close(dp->fd);
        free(dp);
    }
}
