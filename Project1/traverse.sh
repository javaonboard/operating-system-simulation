#!/bin/bash
directory=$1
log=$2
find $directory -type f -print0 | while read -d $'\0' file; 
	do echo "Printing ... $file" >> $log 
done
