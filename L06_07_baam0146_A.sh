#!/bin/bash

# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: Adott egy állomány egy vizsga eredményeivel. Formátuma:
#        Név Keresztnév felhasználónév média
# Ezt az állományt használva bemenetként, írjunk egy levelet minden
# vizsgázónak. Annak, aki átmenõ jegyet kapott, ezt írjuk:
#        Kedves Név Keresztnév!
#        A(z) tantárgy vizsgán elért osztályzata: ...
# Aki nem ment át, annak pedig azt írjuk, hogy:
#        Kedves Név Keresztnév!
#        Jöjjön legközelebb a(z) ... tantárgyból vizsgázni.
# ahol a tantárgy a parancssorban van megadva, a többi információt
# pedig a bemeneti állományból kapjuk.
# A könnyebb ellenõrizhetõség kedvéért az elküldött szövegeket az
# aktuális rendszeridõ kíséretében írjuk bele egy tantargy_log nevű állományba is.

if [ $# -ne 2 ]
then
	echo 'First parameter: name of class'
        echo 'Second parameter: one file with names and averages'
        exit 1
fi

awk -v tantargy=$1 '
BEGIN { print strftime("%Y-%m-%d %H:%M:%S") > "tantargy_log" }
{
if ($4 > 4.5) {
	szoveg="Kedves "$1" "$2"!\nA(z) "tantargy" vizsgan elert osztalyzata: "$4
	system("echo \""szoveg"\"|mail -s \"Vizsga\" " $3)
	print szoveg > "tantargy_log"
}
else {
	szoveg="Kedves "$1" "$2"!\nJojjon legkozelebb a(z) "tantargy" tantargybol vizsgazni."
	system("echo \""szoveg"\"|mail -s \"Vizsga\" " $3)
	print szoveg > "tantargy_log"
}
}' $2
