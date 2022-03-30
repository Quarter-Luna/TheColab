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
CFLAGS= -Wall -ggdb

.PHONY: all

all:wytar

wytar: wytar.o tar.o untar.o general.o regular.o dir.o
	$(CC) $(CFLAGS) *.o -o wytar

wytar.o: wytar.c wytar.h 
	$(CC) -c $(CFLAGS) wytar.c -o wytar.o

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
