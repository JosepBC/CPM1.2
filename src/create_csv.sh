#!/bin/bash

for file in ../out/teen/*
do
    file_name=$(basename $file | cut -f 1 -d .)
    time=$(grep real $file | cut -f 2 -d ' ')
    threads=$(echo $file_name | cut -f 2 -d _)
    echo "$file_name;$time;$threads"
done