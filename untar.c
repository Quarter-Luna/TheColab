#include <stdio.h>
#include "star.h"
void untar(char *name)
{
    FILE *tarptr;
    tarptr = fopen(name, "rb");
    struct header_ustar h;
    fread(&h, sizeof(struct header_ustar), 1, tarptr);
    while (isvalid_tar(&h))
    {
        switch (h.typeflag)
        {
        case '0':
            regular_untar(tarptr, &h);
            break;
        case '5':
            dir_untar(&h);
        }
        fread(&h, sizeof(struct header_ustar), 1, tarptr);
    }
}