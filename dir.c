#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "star.h"
void dir_untar(struct header_ustar *h)
{
    mode_t mode;
    char modeoct[9] = {0};
    strncpy(modeoct, h->mode, 8);
    sscanf(modeoct, "%o", &mode);
    char filename[260] = {0};
    if (h->prefix[0])
    {
        strncat(filename, h->prefix, 155);
        strcat(filename, "/");
    }
    strncat(filename, h->name, 100);
    mkdir(filename, mode);
}
void dir_tar(FILE *tarf, char *dirname, struct stat *s)
{
    struct header_ustar h;
    bzero(&h, sizeof(struct header_ustar));
    int len = (int)strlen(dirname);
    if (len > 255)
        return;
    if (len > 100)
    {
        int j = 154;
        while (dirname[j] != '/')
            j--;
        if (len - j > 100)
            return;
        strncpy(h.prefix, dirname, (size_t)j);
        strncpy(h.name, dirname + j + 1, (size_t)(len - j - 1));
    }
    else
        strcpy(h.name, dirname);
    char *buf = malloc(sizeof(char) * 512);
    sprintf(buf, "%08o", s->st_uid);
    strncpy(h.uid, buf, 8);
    sprintf(buf, "%08o", s->st_gid);
    strncpy(h.gid, buf, 8);
    sprintf(buf, "%012o", 0);
    strncpy(h.size, buf, 12);
    sprintf(buf, "%012o", (int)s->st_mtime);
    strncpy(h.mtime, buf, 12);
    h.typeflag = '5';
    sprintf(buf, "%08o", s->st_mode);
    strncpy(h.mode, buf, 8);
    strcpy(h.magic, MAGIC);
    strncpy(h.version, "00", 2);
    unsigned int chksum = checksum(&h);
    sprintf(h.checksum, "%06o", chksum);
    h.checksum[7] = ' ';
    fwrite(&h, sizeof(struct header_ustar), 1, tarf);

    DIR *dp;
    dp = opendir(dirname);

    struct dirent *content;
    while ((content = readdir(dp)) != NULL)
    {
        if (strcmp(content->d_name, "..") == 0 || strcmp(content->d_name, ".") == 0)
            continue;
        struct stat s2;
        char filename[255] = {0};
        strcat(filename, dirname);
        int l2 = (int)strlen(filename);
        if (filename[l2 - 1] != '/')
        {
            filename[l2] = '/';
            filename[l2 + 1] = 0;
        }
        strcat(filename, content->d_name);
        stat(filename, &s2);
        if (S_ISREG(s2.st_mode))
            regular_tar(tarf, filename, &s2);
        else if (S_ISDIR(s2.st_mode))
            dir_tar(tarf, filename, &s2);
    }
    closedir(dp);
}