#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: A parancssorban megadott állományokban minden magánhangzót
# változtasson meg, a kisbetűt nagyra, a nagyot kicsire.

if [ $# -lt 1 ]
then
        echo 'Parameters: files'
        exit 1
fi

for param in $*
do
	sed 'y/aeiouAEIOU/AEIOUaeiou/' $param
done
