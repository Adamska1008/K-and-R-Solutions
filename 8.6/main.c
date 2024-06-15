#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>

#include "dirent.h"

void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    linux_dirent *dp;
    Dir *dfd;

    if ((dfd = opendir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    while ((dp = readdir(dfd)) != NULL)
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
    closedir(dfd);
}

void fsize(char *name)
{
    struct stat stbuf;
    struct passwd *pw;
    // get state
    if (stat(name, &stbuf) == -1)
    {
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }
    pw = getpwuid(stbuf.st_uid);
    if ((stbuf.st_mode & __S_IFMT) == __S_IFDIR)
        dirwalk(name, fsize);
    printf("%s %8ld %s\n", pw->pw_name, stbuf.st_size, name);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else
        while (argc--)
            fsize(*++argv);
    return 0;
}
