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

max=-1
count=0
for param in $*
do
	id -u $param &> /dev/null
	if [ $? -eq 0 ]
	then
		processes=`ps -u $param |wc -l`
		echo $param ': ' $processes
		if [ $processes -gt $max ]
		then
			max=$processes
			count=1
			users[0]=$param
		elif [ $processes -eq $max ]
		then
			users[$count]=$param
			count=$count+1
		fi
	else
		echo "No such username as $param"
	fi
done

echo 'Users with most processes: '
for i in "${users[@]}"
do
	echo "${users[$i]} is running:"
        ps -o "comm" -u $i | sort -u
done
