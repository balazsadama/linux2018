#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: Írassa ki a paraméterként megadott katalógusban és alaktalógusaiban
# szereplő szöveges file-ok "."-al vagy számjeggyel kezdődő sorait.

if [ $# -ne 1 ]
then
        echo 'Parameter: directory'
        exit 1
fi

if [ -d $1 ]
then
	#find $1 -type f
	files=`find $1 -type f`
	for f in $files
	do
		grep '^[0-9]\|^\.' $f
	done

#	echo $1
#	for d in ~/labor4_grep/level1/*; do
#		if [ -f "$d" ] then
#			echo "$d"
#		fi
#	done
else
	echo 'Invalid directory given.'
fi
