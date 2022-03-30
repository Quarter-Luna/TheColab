#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "star.h"
int file_exist(char *path)
{
    FILE *fp;
    fp = fopen(path, "r");
    if (fp)
    {
        fclose(fp);
        return TRUE;
    }
    return FALSE;
}
int isvalid_tar(const struct header_ustar *h)
{
    unsigned int n;
    sscanf(h->checksum, "%o", &n);
    return n == checksum(h) &&
           (strcmp(h->magic, MAGIC) + 1) % 2;
}
unsigned int checksum(const struct header_ustar *h)
{
    unsigned int chksum = ' ' * 8;
    unsigned int i;
    const unsigned char *hp = (unsigned char *)h;
    for (i = 0; i < offsetof(struct header_ustar, checksum); i++)
        chksum += hp[i];
    for (i += (unsigned int)sizeof(char) * 8; i < sizeof(struct header_ustar); i++)
        chksum += hp[i];
    return chksum;
}