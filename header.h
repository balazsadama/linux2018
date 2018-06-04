/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: A kliens információt kér állományokról. A szerver ellenőrzi, hogy létezik-e az adott állomány, és válaszként visszaküldi az
   állomány típusát, méretét, hány különböző neven hivatkoznak rá, szöveges állományok esetében pedig az állomány sorainak és szavainak számát is. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SZFIFONAME "baam0146_szfifo"
#define KLFIFONAME "baam0146_klfifo"
#define MAXLEN 1024
