#!/bin/bash

if [ -z "$1" ]; then
	echo "Ошибка: указан неправильный файл, для которого надо искать символьческие ссылки"
	exit 1
fi

if [ -z "$2" ]; then
	echo "Ошибка: указан неправильный выходной файл"
	exit 1
fi

file=$(realpath "$1")
out_file=$2
> "$out_file"

sudo ls -lR / 2>/dev/null | grep " -> " | awk -v target="$file" -v counter=0 '
{
	link_path = $NF
	if (link_path == target){
		print $0
		counter++
	}
}
END {
	print counter
}' | tee -a "$out_file"

