#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: Írjon shell script-et, amely kiírja a paraméterként megadott
#felhasználók folyamatainak a számát "felhasználónév: folyamatszám" formában.
#Kiírja, hogy melyik felhasználó (vagy felhasználók) futtatják a legtöbb
#folyamatot, és ezen felhasználó(k) esetén írjuk is ki a futtatott folyamatokat
#úgy, hogy (egy adott felhasználó esetén) mindegyik folyamat csak egyszer szerepeljen. 

if [ $# -lt 1 ]
then
	echo 'parameters: usernames'
	exit 1
fi

for param in $*
do
	id -u $param &> /dev/null
	if [ $? -eq 0 ]
	then
		echo $param ': ' `ps -u $param |wc -l`
	else
		echo "No such username as $param"
	fi
done
