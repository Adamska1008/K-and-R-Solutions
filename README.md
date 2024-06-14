# K-and-R-Solutions
Solutions to exercise in ch 8.

## 8.5

`_flushbuf` also checks if `fp->base` is initialized first. After that, it will write everything in buffer to fd and set `fp->ptr` to `fp->base`. Simple logic.

`fflush(fp)` will call `_flushbuf(EOF, fp)`. `fclose` will call `fflush` and system call of `close`.

`fseek` is designed in a simple way. Each call of `fseek` will flush the buffer in fp.

## 8.6

Failed due to redeclaration of `opendir`, `closedir` `readdir` after include `sys/dir.h`.