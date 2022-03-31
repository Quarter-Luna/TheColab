//
// wytar.c
// Author: Zachary Crimmel
// Date: Mar 29, 2022
//
// COSC 3750, Homework 6
//
// This is a wytar program designed to tar multiple different filesystems objects given by the user
// Collaborated with Ian Moon on this Homework
//

#include <stdio.h>

#include "tar.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int main(int argc, char *argv[])
{
    if (((argc == 2) && (strncmp(argv[1], "help", MAX(strlen(argv[1]), 4)))) || (argc < 3))
    {
        fprintf(stderr, "Usage: %s option(s) tarfile [sources]\n", argv[0]);
        fprintf(stderr, "Usage: %s help\n", argv[0]);
        return -1;
    }

    argc -= 3;

    int rc = 0;
    char c = 0,          // create
        x = 0;          // extract
        f = 0;          // filename

    // parse options
    for (int i = 0; argv[1][i]; i++)
    {
        switch (argv[1][i])
        {
        case 'c':
            c = 1;
            break;
        case 'x':
            x = 1;
            break;
        case 'f' :
            f = 1;
            break;
        case '-':
            break;
        default:
            fprintf(stderr, "Error: Bad option: %c\n", argv[1][i]);
            fprintf(stderr, "Do '%s help' for help\n", argv[0]);
            return 0;
            break;
        }
    }

    // make sure only one of these options was selected
    const char used = c + x;
    if (used > 1)
    {
        fprintf(stderr, "Error: Cannot have so all of these flags at once\n");
        return -1;
    }
    else if (used < 1)
    {
        fprintf(stderr, "Error: Need either c or x options set\n");
        return -1;
    }

    const char *filename = argv[2];
    const char **files = (const char **)&argv[3];

    // //////////////////////////////////////////

    struct tar_t *archive = NULL;
    int fd = -1;
    if (c)
    { // create new file
        if ((fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
        {
            fprintf(stderr, "Error: Unable to open file %s\n", filename);
            return -1;
        }

        if (tar_write(fd, &archive, argc, files) < 0)
        {
            rc = -1;
        }
    }
    else
    {
        // open existing file
        if ((fd = open(filename, O_RDWR)) < 0)
        {
            fprintf(stderr, "Error: Unable to open file %s\n", filename);
            return -1;
        }

        // read in data
        if (tar_read(fd, &archive) < 0)
        {
            tar_free(archive);
            close(fd);
            return -1;
        }

        // perform operation
        if ((x && (tar_extract(fd, archive, argc, files) < 0))       // extract entries
        )
        {
            fprintf(stderr, "Exiting with error due to previous error\n");
            rc = -1;
        }
    }

    tar_free(archive);
    close(fd); // don't bother checking for fd < 0
    return rc;
}