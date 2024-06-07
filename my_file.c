#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_file.h"

FILE _iob[OPEN_MAX] = {
    {0, (char *)0, (char *)0, _READ, 0},
    {0, (char *)0, (char *)0, _WRITE, 1},
    {0, (char *)0, (char *)0, _WRITE | _UNBUF, 2},
};

FILE *fopen(char *name, char *mode)
{
    int fd;
    FILE *fp = (FILE *)malloc(sizeof(FILE));
    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a')
    {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        fd = lseek(fd, 0L, 2);
    }
    else
        fd = open(name, O_RDONLY, 0);
    if (fd == -1)
        return NULL;
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

int fflush(FILE *fp)
{
    _flushbuf(EOF, fp);
}

int fclose(FILE *fp)
{
    if (close(fp->fd) == -1)
        return EOF;
    fflush(fp);
    return 0;
}

int _fillbuf(FILE *fp)
{
    int bufsize;
    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = (char *)malloc(bufsize)) == NULL)
            return EOF;
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->base, bufsize);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
            fp->flag |= _EOF;
        else
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char)*fp->ptr++;
}

int _flushbuf(int ch, FILE *fp)
{
    int bufsize;
    if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = (char *)malloc(bufsize)) == NULL)
            return EOF;
    if (fp->ptr == NULL) // uninitialized
    {
        fp->cnt = bufsize - 1; // so that the variable 'ch' can always be stored
        fp->ptr = fp->base;
    }
    if (ch != EOF)
        *fp->ptr++ = ch;
    int len = fp->ptr - fp->base;
    int wc = write(fp->fd, fp->base, len);
    if (wc != len)
    {
        if (wc == -1)
            fp->flag |= _ERR;
        return EOF;
    }
    fp->ptr = fp->base;
    fp->cnt = bufsize - 1;
    return ch;
}