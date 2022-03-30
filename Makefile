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
RM= rm -f

.PHONY: all clean tidy

all: wytar

wytar: wytar.o tar.o
	$(CC) $(CFLAGS) wytar.o tar.o -o wytar

wytar.o: wytar.c
	$(CC) $(CFLAGS) -c wytar.c

tar.o: tar.c
	$(CC) $(CFLAGS) -c tar.c

clean:
	${RM} *.o wytar

tidy:
	${RM} a.out core.* wytar