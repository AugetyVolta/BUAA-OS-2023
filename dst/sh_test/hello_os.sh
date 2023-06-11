#!/bin/bash
file1=$1
file2=$2
touch $file2
sed -n '8p' $file1>>$file2
sed -n '32p' $file1>>$file2
sed -n '128p' $file1>>$file2
sed -n '512p' $file1>>$file2
sed -n '1024p' $file1>>$file2 

