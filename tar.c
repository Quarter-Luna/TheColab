#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "wytar.h"
void tar(int c, char **names)
{
    int i;
    FILE *tar_file;
    tar_file = fopen(names[0], "wb");
    for (i = 1; i < c; i++)
    {
        struct stat s;
        stat(names[i], &s);
        if (S_ISREG(s.st_mode))
            regular_tar(tar_file, names[i], &s);
        else if (S_ISDIR(s.st_mode))
            dir_tar(tar_file, names[i], &s);
    }
    for (i = 0; i < 256; i++)
    {
        int z = 0;
        fwrite(&z, sizeof(int), 1, tar_file);
    }
    fclose(tar_file);
}