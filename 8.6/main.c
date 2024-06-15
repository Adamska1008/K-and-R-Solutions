#include "dirent.h"
// #define _GNU_SOURCE
// #include <sys/syscall.h>
// #include <sys/types.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <dirent.h>
// #include <unistd.h>
// #include <errno.h>
// #include <stdio.h>

// struct linux_dirent
// {
//     long d_ino;
//     off_t d_off;
//     unsigned short d_reclen;
//     char d_name[];
// };

// #define BUF_SIZE 1024

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else
        while (argc--)
            fsize(*++argv);
    return 0;
    // int fd = open(".", O_RDONLY);
    // char buf[BUF_SIZE];
    // struct linux_dirent *d;
    // for (;;)
    // {
    //     int nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
    //     if (nread == -1)
    //     {
    //         perror("syscall(SYS_getdents)");
    //         exit(EXIT_FAILURE);
    //     }
    //     if (nread == 0)
    //         break;
    //     for (int bpos = 0; bpos < nread;)
    //     {
    //         d = (struct linux_dirent *)(buf + bpos);
    //         printf("inode: %ld, offset: %lld, len: %d, name: %s\n",
    //                d->d_ino, (long long)d->d_off, d->d_reclen, d->d_name);
    //         bpos += d->d_reclen;
    //     }
    // }
    // close(fd);
    // return 0;
}
