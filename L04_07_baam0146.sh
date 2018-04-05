#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: A parancssorban megadott szerverekre írassa ki, hogy elérhető-e
# vagy nem. (Annak eldöntésére, hogy egy szerver elérhető-e vagy sem a
# ping parancsot használhatjuk.)

if [ $# -lt 1 ]
then
        echo 'parameters: servers'
        exit 1
fi

for param in $*
do
	badInput=`ping -q -c3 $param 2>&1 |grep -c 'Name or service not known'`
	if [ $badInput -eq 1 ]
	then
		echo 'Invalid server: ' $param
	else
		output=`ping -q -c3 $param |grep -c '0 received'`
		
		if [ $output -eq 0 ]
		then
			echo $param ' is reachable'
		else
			echo $param ' is unreachable'
		fi
	fi
done
