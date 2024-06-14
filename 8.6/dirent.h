#ifndef DIRENT_H
#define DIRENT_H
#define MAX_PATH 1024

typedef struct
{
    long ino;
    char name[256]; // name + '\0'
} Dirent;

typedef struct
{
    int fd;
    Dirent d;
} Dir;

Dir *open_dir(char *dirname);
Dirent *read_dir(Dir *dfd);
void close_dir(Dir *dfd);

void fsize(char *);

#endif