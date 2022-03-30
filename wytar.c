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
#include "wytar.h"
int main(int argc, char **argv)
{
    switch (argc)
    {
    case 1:
        untar(argv[1]);
        break;
    default:
        tar(argc - 1, &argv[1]);
    }
    argv++;
    return 0;
}