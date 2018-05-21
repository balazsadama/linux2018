/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat:Adott az x1, x2, ..., xn számsorozat. A divide et impera módszert alkalmazva rendezzük a sorozatot: a folyamat a számsorozatot két részre
   osztja, amelyeket két gyerekfolyamatának ad át, hogy rendezzék, a visszakapott eredményeket pedig összefésüli. A gyerekfolyamatok ugyanezt a
   módszert alkalmazzák. (Mivel korlátozott az elindítható folyamatok száma, csak kevés számra fog működni.) n-et és a sorozat elemeit az „input.dat”
   állományból olvassuk be.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void merge(int* x, int a, int b, int c){
	int *temp, i, j, k, n;

	n = (b - a) + (c - b) + 2;
	temp = (int*)malloc(n * sizeof(int));
	i = a;
	j = b + 1;
	k = 0;

	while (i <= b && j <= c){
		if (x[i] < x[j])
			temp[k++] = x[i++];
		else
			temp[k++] = x[j++];
	}

	while (i <= b)
		temp[k++] = x[i++];
	while (j <= c)
		temp[k++] = x[j++];


	for (i = a; i <= c; i++)
		x[i] = temp[i - a];

	free(temp);
}

void mergeSort(int *x, int a, int b){
	pid_t child1 = -1, child2 = -1, root = -1;
	int i, k, iterator = 0;
	int pfdL[2], nL, pfdR[2], pfdP[2], nR;

	pipe(pfdP);
	root = fork();

	if (root == 0) {

	while (iterator < 10) {
		// we create 2 child processes to sort left and right part of array
		k = a + (b - a) / 2;

		pipe(pfdL);
		child1 = fork();

		if (child1 == 0) { // sort left
			// read data sent by parent
			read(pfdL[0], &nL, sizeof(int));
			int *left = (int*)malloc(nL * sizeof(int));

			printf("Child %d receiving from parent %d values ", getpid(), getppid());
			for (i = 0; i < nL; i++) {
				read(pfdL[0], left + i, sizeof(int));
				printf("%d ", left[i]);
			}
			printf("\n");

			if (nL == 1) {
				// if only 1 element received, send it back
				printf("Child %d sending to parent %d values %d\n", getpid(), getppid(), left[0]);
				write(pfdL[1], left, sizeof(int));
				free(left);
				exit(0);
			}
			else if (nL <= 0)
				exit(1);
			else {
				// else, backtrack
				b = k;
				pfdP[0] = pfdL[0];
				pfdP[1] = pfdL[1];
			}
		}
		else { 
			
			pipe(pfdR);
			child2 = fork();

			if (child2 == 0) { // sort right
				// read data sent by parent
				read(pfdR[0], &nR, sizeof(nR));
				int *right = (int*)malloc(nR * sizeof(int));

				printf("Child %d receiving from parent %d values ", getpid(), getppid());
				for (i = 0; i < nR; i++) {
					read(pfdR[0], right + i, sizeof(int));
					printf("%d ", right[i]);
				}
				printf("\n");

				if (nR == 1) {
					// if only 1 element received, send it back
					printf("Child %d sending to parent %d values %d\n", getpid(), getppid(), right[0]);
					write(pfdR[1], right, sizeof(int));
					free(right);
					exit(0);
				}
				else if (nR <= 0)
					exit(1);
				else {
					// else, backtrack
					a = k + 1;
					pfdP[0] = pfdR[0];
					pfdP[1] = pfdR[1];
				}
			}

			else {
				// send info to both children, then merge it together
				k = a + (b - a) / 2;

				if (a > b)
					break;

				nL = k - a + 1;
				write(pfdL[1], &nL, sizeof(int));
				for (i = a; i <= k; i++) {
					write(pfdL[1], x + i, sizeof(int));
				}

				nR = b - k;
				write(pfdR[1], &nR, sizeof(int));
				for (i = k + 1; i <= b; i++) {
					write(pfdR[1], x + i, sizeof(int));
				}

				// wait for children to sort
				wait(NULL);

				// read data sent by children and merge it
				int m, temp;
				for (m = a; m <= k; m++) {
					read(pfdL[0], &temp, sizeof(int));
					x[m] = temp;
				}
				for (m = k + 1; m <= b; m++) {
					read(pfdR[0], &temp, sizeof(int));
					x[m] = temp;
				}

				merge(x, a, k, b);

				// send merged result to parent
				printf("Child %d sending to parent %d values ", getpid(), getppid());
				for (m = a; m <= b; m++) {
					write(pfdP[1], x + m, sizeof(int));
					printf("%d ", x[m]);
				}
				printf("\n");

				exit(0);
				
			}
		}
		iterator++;
	}
		exit(0);
	}
	else {
		close(pfdL[0]);
		close(pfdL[1]);
		close(pfdR[0]);
		close(pfdR[1]);
		// wait for the array to be sorted, then print
		wait(NULL);

		int m;
		printf("Root %d receiving from %d values ", getpid(), root);
		for (m = a; m <= b; m++){
			read(pfdP[0], x + m, sizeof(int));
			printf("%d ", x[m]);
		}
		printf("\n");
		return;
	}
}


int main(void){
	int *arr, n, i;
	FILE *fin, *fout;

	fin = fopen("input.dat", "r");
	fscanf(fin, "%d ", &n);
	arr = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++)
		fscanf(fin, "%d ", arr + i);

	fclose(fin);
	mergeSort(arr, 0, n - 1);
	
	fout = fopen("output.dat", "w");
	for (i = 0; i < n; i++)
		fprintf(fout, "%d ", arr[i]);

	fclose(fout);
	free(arr);
	return 0;
}

