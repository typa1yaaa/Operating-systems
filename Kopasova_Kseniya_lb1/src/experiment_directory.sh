#!/bin/bash

mkdir -p experiment_testdir
stat experiment_testdir | head -n 3
for i in {1..1000}; do
	touch experiment_testdir/file$i
done
echo "В experiment_testdir добавлено 1000 файлов"
stat experiment_testdir | head -n 3
rm -f experiment_testdir/file*
echo "Из experiment_testdir удалено 1000 файлов"
stat experiment_testdir | head -n 3
rm -rf experiment_testdir
