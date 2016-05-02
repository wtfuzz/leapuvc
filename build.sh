#!/bin/sh

CFLAGS="-I/usr/local/include -O3"
LDFLAGS="-L/usr/local/lib"
LIBRARY_PATH="/usr/local/lib"

gcc $CFLAGS -c main.c
gcc $CFLAGS -c leap.c
gcc $CFLAGS -c leap_calibrate.c
gcc $LDFLAGS -o main main.o leap.o leap_calibrate.o -luvc -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d

g++ $LDFLAGS $CFLAGS -o detect detect.cpp leap.o -luvc -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_nonfree
g++ $LDFLAGS $CFLAGS -o calib calib.cpp leap.o -luvc -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_nonfree
