#!/bin/sh

CFLAGS="-I/usr/local/include -O3"
LDFLAGS="-L/usr/local/lib"
LIBRARY_PATH="/usr/local/lib"

gcc $CFLAGS -c main.c
gcc $LDFLAGS -o main main.o -luvc -lopencv_highgui -lopencv_core -lopencv_imgproc
