#!/bin/sh
file=$1
make -f makefile
./cal.exe < cal.in >> $file
./cal.exe < cal.in | wc >> $file
