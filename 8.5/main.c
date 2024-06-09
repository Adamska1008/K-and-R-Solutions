#include "my_file.h"

int main()
{
    FILE *fp = fopen("textfile", "r");
    int c;
    fseek(fp, 2, 0);
    while ((c = getc(fp)) != EOF)
        putc(c, stdout);
    fflush(stdout);
    fclose(fp);
}