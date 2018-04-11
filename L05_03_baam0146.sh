#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: A parancssorban megadott állományok első 30 sorából törölje
# ki azokat a sorokat, amelyek az első paraméterként megadott szöveget
# tartalmazzák.

if [ $# -lt 2 ]
then
	echo 'First parameter: a pattern text'
	echo 'Following paramters: files'
	exit 1
fi

word=$1
shift

for param in $*
do
	if [ ! -f $param ]
	then
		echo $param ' is not a valid file'
	else
		echo $param
	fi
done
