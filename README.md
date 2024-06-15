# K-and-R-Solutions
Solutions to exercise in ch 8.

## 8.5

`_flushbuf` checks if `fp->base` is initialized first. After that, it will write everything in buffer to fd and set `fp->ptr` to `fp->base`. Simple logic. Detail: the `cnt` will be set to `BUFSIZE - 1` so that the `char` arg will always fit in buffer and be flushed together.  

`fflush(fp)` calls `_flushbuf(EOF, fp)`. `fclose` calls `fflush` and system call `close`.

`fseek` is designed in a simple way. Each call of `fseek` flushes the buffer in fp.

## 8.6

So the example code is basically not appliable in current linux system. To read dirent, you should first define a `linux_dirent` by yourself and call `syscall(SYS_getdent)` to fill it. If you try to call `read` on a directory, you will get an error 21.

The exercise in this chapter is relatively easy. Use `man 2 stat` to check what you can get in `struct stat` and print them. Here I use `st_uid` to print the owner name. 
