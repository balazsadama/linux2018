#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024

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
	char buf[MAXLINE];

	root = fork();

	if (root == 0) {
	while (iterator < 10) {
		// we create 2 child processes to sort left and right part of array
		k = a + (b - a) / 2;


		pipe(pfdL);
		child1 = fork();

		if (child1 == 0) { // sort left
			read(pfdL[0], &nL, sizeof(nL));
			int *left = (int*)malloc(nL * sizeof(int));
			//printf("%dparent %dchild left received: nL=%d\t", getppid(), getpid(), nL);
			for (i = 0; i < nL; i++) {
				read(pfdL[0], left + i, sizeof(i));
			//	printf("%d ", left[i]);
			}
			//printf("\n");
			if (nL == 1) {
				write(pfdL[1], left, sizeof(left[0]));
				printf("%dparent %dchild exited %d\n", getppid(), getpid(), left[0]);
				free(left);
			//	printf("\n\n%d\n\n", left[0]);
				exit(0);
			}
			else {
				b = k;
				// CHANGED HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEee
				pfdP[0] = pfdL[0];
				pfdP[1] = pfdL[1];
			}
		}
		else { 
			
			pipe(pfdR);
			child2 = fork();

			if (child2 == 0) { // sort right
				read(pfdR[0], &nR, sizeof(nR));
				int *right = (int*)malloc(nR * sizeof(int));
				//printf("%dparent %dchild right received: nR=%d\t", getppid(), getppid(), nR);
				for (i = 0; i < nR; i++) {
					read(pfdR[0], right + i, sizeof(i));
				//	printf("%d ", right[i]);
				}
				//printf("\n");
				if (nR == 1) {
					write(pfdR[1], right, sizeof(right[0]));
					printf("%dparent %dchild exited %d\n", getppid(), getpid(), right[0]);
					free(right);
					//printf("\n\n%d\n\n", right[0]);
					exit(0);
				}
				else {
					a = k + 1;
					// AND HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
					pfdP[0] = pfdR[0];
					pfdP[1] = pfdR[1];
				}
			}

			else {
				// send info to both children, then merge it together
				k = a + (b - a) / 2;

				if (a > b)
					break;

				//printf("%d %d %d %d ", getpid(), a, b, k);
				nL = k - a + 1;
				//printf("sending left nL=%d ", nL);
				write(pfdL[1], &nL, sizeof(nL));
				for (i = a; i <= k; i++) {
					write(pfdL[1], x + i, sizeof(x[i]));
				//	printf("x[%d]=%d ", i, x[i]);
				}
			
				nR = b - k;
				//printf(" sending right nR=%d ", nR);
				write(pfdR[1], &nR, sizeof(nR));
				for (i = k + 1; i <= b; i++) {
					write(pfdR[1], x + i, sizeof(x[i]));
				//	printf("x[%d]=%d ", i, x[i]);
				}
				//printf("\n");

				wait(NULL);
//				wait(NULL);

				// most kiolvasom a gyerekek altal kuldott adatokat
				int m, temp;
				printf("%dparent read from left\t", getpid());
				for (m = a; m <= k; m++) {
					read(pfdL[0], &temp, sizeof(temp));
					x[m] = temp;
					printf("x[%d]=%d ", m, temp);
				}
				printf("\tfrom right:\t");
				for (m = k + 1; m <= b; m++) {
					read(pfdR[0], &temp, sizeof(temp));
					x[m] = temp;
					printf("x[%d]=%d ", m, temp);
				}
				printf("\n");

				for (m = a; m <= b; m++) 
					printf("x[%d]=%d ", m, x[m]);
				merge(x, a, k, b);
				for (m = a; m <= b; m++) {
					printf("x[%d]=%d ", m, x[m]);
				}
				printf("\n");

				// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
				for (m = a; m <= b; m++) {
					write(pfdP[1], x + m, sizeof(x[m]));
				}

				exit(0);
				
			}
		}
		iterator++;
	}
	}
	else {
		close(pfdL[0]);
		close(pfdL[1]);
		close(pfdR[0]);
		close(pfdR[1]);
		// wait for the array to be sorted, then print
		wait(NULL);



		printf("\n\n\n\n");
		int m;
		for (m = a; m <= b; m++){
			printf("%d ", x[m]);
		}
		printf("\n");
		exit(1);
	}
}


int main(void){
//	int t[6] = {1, 3, 5, 2, 4, 6};
	int t[6] = {1, 5, 3, 4, 2, 7};

/*	for (int i = 0; i < 5; i++) 
		printf("%d ", t[i]);
*/
	mergeSort(t, 0, 5);

	/*
	printf("\n");
	for (int i = 0; i < 5; i++) 
                printf("%d ", t[i]);
*/
	return 0;
}

