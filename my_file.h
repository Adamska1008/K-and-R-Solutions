#ifndef MY_FILE_H
#define MY_FILE_H

#define PERMS 0666
#define NULL 0
#define EOF (-1)
#define BUFSIZ 1024
#define OPEN_MAX 20

typedef struct _iobuf
{
    int cnt;    // chars left
    char *ptr;  // next char pos
    char *base; // buf loc
    int flag;   // file access
    int fd;
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdio (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum _flags
{
    _READ = 01,
    _WRITE = 02,
    _UNBUF = 04,
    _EOF = 010,
    _ERR = 020,
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0) ? *(p)->ptr++ = (x) : _flushbuf((x), (p))

FILE *fopen(char *, char *);
int fflush(FILE *);
int fclose(FILE *);

#endif