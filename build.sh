#!/bin/sh

CFLAGS="-I/usr/local/include -O3"
LDFLAGS="-L/usr/local/lib"
LIBRARY_PATH="/usr/local/lib"

gcc $CFLAGS -c main.c
gcc $CFLAGS -c leap.c
gcc $CFLAGS -c leap_calibrate.c
gcc $LDFLAGS -o main main.o leap.o -luvc -lopencv_highgui -lopencv_core -lopencv_imgproc
