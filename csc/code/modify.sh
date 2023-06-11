#!/bin/bash
file=$1
src=$2
dst=$3
sed -i "s/$src/$dst/g"  $file
