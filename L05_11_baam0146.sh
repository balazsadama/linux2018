#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: A parancssorban megadott html állományt alakítsa egyszerű
# szöveges állománnyá (az állomány tartalmából töröljön mindent, ami
# „<” és „>” karakterek között van).

if [ $# -ne 1 ]
then
        echo 'Parameter: html file'
        exit 1
fi

sed 's/<[^>]\+>//g' $1
