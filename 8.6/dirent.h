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
} DIR;

DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);

void fsize(char *);

#endif