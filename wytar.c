//
// wytar.c
// Author: Zachary Crimmel
// Date: Mar 29, 2022
//
// COSC 3750, Homework 6
//
// This is a wytar program designed to tar multiple different filesystems objects given by the user
// Collaborated with Ian Moon on this Homework
// Used the tar program found at https://github.com/calccrypto/tar/blob/master/main.c to complete this homework
// Copyright (c) 2015 Jason Lee
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

    if (argc == 2)
    {
        fprintf(stdout, "Usage: %s options(s) tarfile [sources]\n"
                        "Usage: %s help\n"
                        "\n"
                        "Important:\n"
                        "    This program is not meant to be a full tar implementation.\n"
                        "    Only a subset of the functions the GNU tar utility has are supported.\n"
                        "\n"
                        "    Special files that already exist will not be replaced when extracting (no error)\n"
                        "    Regular expression expansion/matching is not done.\n"
                        "\n"
                        "    options (only one allowed at a time):\n"
                        "        c - create a new archive\n"
                        "        x - extract from archive\n"
                        "\n"
                        "    other options:\n"
                        "        v - make operation verbose\n"
                        "\n"
                        "Ex: %s vl archive.tar\n",
                argv[0], argv[0], argv[0]);
        return 0;
    }

    argc -= 3;

    int rc = 0;
    char c = 0,         // create
        x = 0,          // extract
        f = 0;
    char verbosity = 0; // 0: no print; 1: print file names; 2: print file properties

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
        case 'v':
            verbosity++;
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
    for (int i = 0; argv[2][i]; i++)
    {
        switch (argv[2][i])
        {
        case 'f':
            f = '1';
            break;
        case '-':
            break;
        default:
            fprintf(stderr, "Error: Bad option, or -f not used, please use -f to declare the archive being used\n");
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
        fprintf(stderr, "Error: Need one of 'cx' options set\n");
        return -1;
    }

    if (f != 1){
        fprintf(stderr, "Error: Must use -f to declare the archive being used\n");
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

        if (tar_write(fd, &archive, argc, files, verbosity) < 0)
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
        if (tar_read(fd, &archive, verbosity) < 0)
        {
            tar_free(archive);
            close(fd);
            return -1;
        }

        // perform operation
        if ((x && (tar_extract(fd, archive, argc, files, verbosity) < 0)) // extract entries
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