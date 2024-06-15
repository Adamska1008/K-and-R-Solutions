#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dirent.h"

void dirwalk(char *, void (*fcn)(char *));

void fsize(char *name)
{
    struct stat stbuf;
    // get state
    if (stat(name, &stbuf) == -1)
    {
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }
    if ((stbuf.st_mode & __S_IFMT) == __S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}

int fstat(int fd, struct stat *);

Dir *open_dir(char *dirname)
{
    int fd;
    struct stat stbuf;
    Dir *dp;

    if ((fd = open(dirname, O_RDONLY, 0)) == -1 || fstat(fd, &stbuf) == -1 || (stbuf.st_mode & __S_IFMT) != __S_IFDIR || (dp = (Dir *)malloc(sizeof(Dir))) == NULL)
        return NULL;
    dp->fd = fd;
    return dp;
}

linux_dirent *read_dir(Dir *dp)
{
    static char buf[BUFSIZ];
    static int bpos;
    static linux_dirent dirent;
    int nread = syscall(SYS_getdents, dp->fd, buf, BUFSIZ);
    if (nread == -1)
    {
        perror("syscall(SYS_getdents)");
        exit(EXIT_FAILURE);
    }
    if (nread == 0)
        return NULL;
    dirent = *(linux_dirent *)(buf + bpos);
    bpos += dirent.d_reclen;
    return &dirent;
}

void close_dir(Dir *dp)
{
    if (dp)
    {
        close(dp->fd);
        free(dp);
    }
}

void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    linux_dirent *dp;
    Dir *dfd;

    if ((dfd = open_dir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    while ((dp = read_dir(dfd)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue; // skip self and parent to avoid loop forever
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s %s is too long\n", dir, dp->d_name);
        else
        {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fcn)(name);
        }
    }
    close_dir(dfd);
}