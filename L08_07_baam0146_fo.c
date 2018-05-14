/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: Adott az x1, x2, ..., xn számsorozat. A divide et impera módszert alkalmazva rendezzük a sorozatot: a folyamat a számsorozatot
   két részre osztja, amelyeket két gyerekfolyamatának ad át, hogy rendezzék, a visszakapott eredményeket pedig összefésüli. A gyerekfolyamatok
   ugyanezt a módszert alkalmazzák. (Mivel korlátozott az elindítható folyamatok száma, csak kevés számra fog működni.) n-et és a
   sorozat elemeit az „input.dat” állományból olvassuk be. */


#include "tavolsag.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *inFile, *outFile;
	int *x, *y;
	int i, n;

	inFile = fopen("input.dat", "r");
	outFile = fopen("output.dat", "w");

	fscanf(inFile, "%d", &n);
	x = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++){
		fscanf(inFile, "%d", x + i);
	}

	fscanf(inFile, "%d", &n);
	y = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++){
		fscanf(inFile, "%d", y + i);
	}

	fprintf(outFile, "%f", tavolsag(x, y, n));

	free(x);
	free(y);
	fclose(inFile);
	fclose(outFile);

	return 0;
}
