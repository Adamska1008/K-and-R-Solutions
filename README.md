# K-and-R-Solutions
Solutions to exercise in ch 8.

## 8.5

`_flushbuf` also checks if `fp->base` is initialized first. After that, it will write everything in buffer to fd and set `fp->ptr` to `fp->base`. Simple logic. Detail: the `cnt` will be set to `BUFSIZE - 1` so that the `char` arg will fit in buffer and be flushed together.  

`fflush(fp)` will call `_flushbuf(EOF, fp)`. `fclose` will call `fflush` and system call of `close`.

`fseek` is designed in a simple way. Each call of `fseek` will flush the buffer in fp.

## 8.6

So the K&R 2th edition is basically not appliable in Ubuntu. To read dirent, you should first define a `linux_dirent` by yourself and call `syscall(SYS_getdent)` to fill it. If you try to call `read` on a directory, you will get an error 21.

The exer in this chapter is relatively easy.
