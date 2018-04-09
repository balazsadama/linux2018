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
#	files=`find $1 -type f`
#	for f in $files
#	do
#		grep '^[0-9]\|^\.' $f
#	done

	grep '^[0-9]\|^\.' -Rh $1

else
	echo 'Invalid directory given.'
fi
