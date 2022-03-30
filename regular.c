#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "star.h"
void regular_untar(FILE *tarf, struct header_ustar *h)
{
    /*getting the file name from prefix/name*/
    char filename[260] = {0};
    if (h->prefix[0])
    {
        strncat(filename, h->prefix, 155);
        strcat(filename, "/");
    }
    strncat(filename, h->name, 100);
    FILE *fp; // output file
    fp = fopen(filename, "w");
    if (fp == NULL)
        return;
    /*write file data*/
    unsigned int filesize;
    sscanf(h->size, "%12o", &filesize);
    char *buffer = malloc(sizeof(unsigned char) * filesize);
    fread(buffer, sizeof(unsigned char), (size_t)(filesize), tarf);
    fwrite(buffer, sizeof(unsigned char), (size_t)filesize, fp);
    long int toseek = 512 - ftell(tarf) % 512;
    if (toseek < 512)
        fseek(tarf, toseek, SEEK_CUR);
    fclose(fp);
}
void regular_tar(FILE *tarf, char *filename, struct stat *filestat)
{
    struct header_ustar h;
    bzero(&h, sizeof(struct header_ustar));
    int len = (int)strlen(filename);
    /*filling in the name,prefix members of the structure*/
    if (len > 255)
        return;
    if (len > 100)
    {
        int j = 154;
        while (filename[j] != '/')
            j--;
        if (len - j > 100)
            return;
        strncpy(h.prefix, filename, (size_t)j);
        strncpy(h.name, filename + j + 1, (size_t)(len - j - 1));
    }
    else
        strcpy(h.name, filename);

    /*getting the uid,gid & rest of the struct members*/
    char *buf = malloc(sizeof(unsigned char) * 512);
    sprintf(buf, "%08o", filestat->st_uid);
    strncpy(h.uid, buf, 8);
    sprintf(buf, "%08o", filestat->st_gid);
    strncpy(h.gid, buf, 8);
    sprintf(buf, "%012o", (unsigned int)filestat->st_size);
    strncpy(h.size, buf, 12);
    sprintf(buf, "%012o", (unsigned int)filestat->st_mtime);
    strncpy(h.mtime, buf, 12);
    h.typeflag = '0';
    sprintf(buf, "%08o", filestat->st_mode);
    strncpy(h.mode, buf, 8);
    strcpy(h.magic, MAGIC);
    strncpy(h.version, "00", 2);
    unsigned int chksum = checksum(&h);
    sprintf(h.checksum, "%06o", chksum);
    h.checksum[7] = ' ';
    fwrite(&h, sizeof(struct header_ustar), 1, tarf);
    FILE *fp;
    fp = fopen(filename, "rb");

    bzero(buf, 512);
    while ((int)fread(buf, sizeof(unsigned char), (size_t)512, fp) != 0)
    {
        fwrite(buf, (size_t)512, sizeof(unsigned char), tarf);
        bzero(buf, 512);
    }
    fclose(fp);
}