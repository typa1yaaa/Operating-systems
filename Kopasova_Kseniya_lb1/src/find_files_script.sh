#!/bin/bash

if [ -z "$1" ]; then
	echo "Ошибка: не задано имя выходного файла"
	exit 1
fi

out_file=$1
> "$out_file"

find_files(){
	local type="$1"
	
	sudo ls -lR / 2>/dev/null | grep "^$type" |  head -n 1 | sudo tee -a "$out_file"

}

find_files "-"
find_files "d"
find_files "c"
find_files "l"
find_files "b"
find_files "p"
find_files "s"

