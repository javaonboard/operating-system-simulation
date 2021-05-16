#!/bin/bash
directory=$1
log=$2
for dir in "$directory"; do
    if [ -d "$dir" ]; then
            echo "directory: $dir" >> $log 
            cd "$dir"
	    for dir2 in ./*; do
	    	if [ -d "$dir2" ]; then
		echo " directory: $dir2" >> $log
			cd "$dir2"
			ls -d "$PWD" /*
			cd ..
		else
		echo " file: $dir2" >> $log
		fi
	done
	cd ..
    else
    echo "file: $dir" >> $log
    fi
done
		
