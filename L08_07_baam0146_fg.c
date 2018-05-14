/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: Adott az x1, x2, ..., xn számsorozat. A divide et impera módszert alkalmazva rendezzük a sorozatot: a folyamat a számsorozatot
   két részre osztja, amelyeket két gyerekfolyamatának ad át, hogy rendezzék, a visszakapott eredményeket pedig összefésüli. A gyerekfolyamatok
   ugyanezt a módszert alkalmazzák. (Mivel korlátozott az elindítható folyamatok száma, csak kevés számra fog működni.) n-et és a
   sorozat elemeit az „input.dat” állományból olvassuk be. */

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
