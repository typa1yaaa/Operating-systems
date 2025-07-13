#!/bin/bash

if [ -z "$1" ]; then
	echo "Ошибка: не задано имя файла для поиска жестких ссылок"
	exit 1
fi

if [ -z "$2" ]; then
	echo "Ошибка: не задано имя выходного файла"
	exit 1
fi

file=$1
out_file=$2

inode=$(ls -i "$file" | awk '{print $1}')

if [ -z "$inode" ]; then
	echo "Ошибка: файл не существует" 
	exit 1
fi

sudo ls -liR / 2>/dev/null | grep "^$inode" | awk '{print}'| sudo tee "$out_file"

