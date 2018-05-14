/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: Írjunk egy „tavolsag(x,y,n)” függvényt, amelyik kiszámítja az x = (xi) és y = (yi), i = 1, ..., n vektorok euklideszi távolságát
   (a vektorok azonos rangú elemei különbsége négyzetösszegének a négyzetgyöke). Ezt a függvényt használjuk 2 vektor távolságának kiszámítására.
   A vektorokat az „input.dat” file-ból olvassuk be, az eredményt pedig, a bemenő adatokkal együtt az „output.dat” file-ba írjuk. */

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
