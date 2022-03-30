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

.PHONY: clean tidy

wytar: wytar.c tar.c tar.h 
	$(CC) $(CFLAGS) *.o -o wytar

clean:
	${RM} wytar

tidy:
	${RM} a.out core.* wytar