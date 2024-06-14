#include <sys/stat.h>
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

Dirent *read_dir(Dir *dp)
{
    struct dirent dirbuf;
    static Dirent d;
    while (read(dp->fd, (char *)&dirbuf, sizeof(dirbuf)) == sizeof(dirbuf))
    {
        if (dirbuf.d_ino == 0) // slot not in use
            continue;
        d.ino = dirbuf.d_ino;
        strcpy(d.name, dirbuf.d_name);
        return &d;
    }
    return NULL;
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
    Dirent *dp;
    DIR *dfd;

    if ((dfd = open_dir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    while ((dp = read_dir(dfd)) != NULL)
    {
        if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
            continue; // skip self and parent to avoid loop forever
        if (strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s %s is too long\n", dir, dp->name);
        else
        {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }
    close_dir(dfd);
}