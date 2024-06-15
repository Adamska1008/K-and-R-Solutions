#include "dirent.h"

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else
        while (argc--)
            fsize(*++argv);
    return 0;
}
