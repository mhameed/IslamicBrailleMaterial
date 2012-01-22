#!/bin/bash

# Extracts chapter number, verse number and text from sql.

cat quran-simple-enhanced.sql | \
grep -i "^(" | \
cut -d, -f2,3,4  | \
sed "s/^(//" | sed -e "s/, '/, /" | 
sed "s/')$//" | sed "s/');\s*$//"  |
sed -e 's/^ \([0-9]*\), \([0-9]*\), /\1.\2 /g' 
