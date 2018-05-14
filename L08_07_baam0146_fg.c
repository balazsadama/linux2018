/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: Írjunk egy „tavolsag(x,y,n)” függvényt, amelyik kiszámítja az x = (xi) és y = (yi), i = 1, ..., n vektorok euklideszi távolságát
   (a vektorok azonos rangú elemei különbsége négyzetösszegének a négyzetgyöke). Ezt a függvényt használjuk 2 vektor távolságának kiszámítására.
   A vektorokat az „input.dat” file-ból olvassuk be, az eredményt pedig, a bemenő adatokkal együtt az „output.dat” file-ba írjuk. */

#include "tavolsag.h"
#include <math.h>

double tavolsag(int* x, int* y, int n){
	double sum = 0;
	int i;

	for (i = 0; i < n; i++){
		sum += (x[i] - y[i]) * (x[i] - y[i]);
	}

	return sqrt(sum);
}
