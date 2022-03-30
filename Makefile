# 
# Makefile
# Author: Zachary Crimmel
# Date: Mar 29, 2022
#
# COSC 3750, Homework 6
#
# This is a Makefile designed to compile the wytar.c file
# Collaborated with Ian Moon on this Homework
#

CC=gcc
CFLAGS= -Wall -Wextra -Wfloat-equal -Wundef -Werror -std=gnu99 -fverbose-asm  -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion
GDBFLAGS= -g -ggdb -ggdb3
.PHONY: all clean docs

remove:
    \rm -rf /opt/wytar /bin/wytar
install: all
    mkdir /opt/wytar/
    cp wytar /opt/wytar
    cp help.doc /opt/wytar
    ln /opt/wytar/wytar /bin/wytar
all:wytar
docs:
    doxygen
wytar: main.o help.o tar.o untar.o general.o regular.o dir.o
    $(CC) $(CFLAGS) *.o -o wytar
    strip wytar
main.o: main.c wytar.h 
    $(CC) -c $(CFLAGS) main.c -o main.o
tar.o: tar.c wytar.h
    $(CC) -c $(CFLAGS) tar.c -o tar.o
untar.o: untar.c wytar.h
    $(CC) -c $(CFLAGS) untar.c -o untar.o
general.o: general.c wytar.h
    $(CC) -c $(CFLAGS) general.c -o general.o
regular.o: regular.c wytar.h
    $(CC) -c $(CFLAGS) regular.c -o regular.o
dir.o: wytar.h
    $(CC) -c $(CFLAGS) dir.c -o dir.o
clean:
    \rm -rf *.o wytar docs