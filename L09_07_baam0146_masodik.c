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
	int i, k, unsorted = 1;
	int pfdL[2], nL, pfdR[2], nR;
	char buf[MAXLINE];

	// open pipe for left child
	pipe(pfdL);
	// open pipe for right child
	pipe(pfdR);

	root = fork();

	if (root == 0) {
	while (1) {
		// we create 2 child processes to sort left and right part of array
		k = a + (b - a) / 2;
		child1 = fork();

		if (child1 == 0) { // if child, then sort left
			read(pfdL[0], &nL, sizeof(nL));
			int *left = (int*)malloc(nL * sizeof(int));
			printf("left received: ");
			for (i = 0; i < nL; i++) {
				read(pfdL[0], left + i, sizeof(i));
				printf("%d ", left[i]);
			}
			if (nL == 1) {
				write(pfdL[1], left, sizeof(left[0]));
				free(left);
				printf("\n\n%d\n\n", left[0]);
				exit(0);
			}
			else {
				b = k;
				//free(left);
			}
		}
		else { 
			
			child2 = fork();

			if (child2 == 0) {
				read(pfdR[0], &nR, sizeof(nR));
				int *right = (int*)malloc(nR * sizeof(int));
				printf("%d right received: ", getpid());
				for (i = 0; i < nR; i++) {
					read(pfdR[0], right + i, sizeof(i));
					printf("%d ", right[i]);
				}
				if (nR == 1) {
					write(pfdR[1], right, sizeof(right[0]));
					free(right);
					printf("\n\n%d\n\n", right[0]);
					exit(0);
				}
				else {
					a = k + 1;
					printf("a = %d\n", a);
					//free(right);
				}
			}

			else {
				// send info to both children, then merge it together
				k = a + (b - a) / 2;
			
				nL = k - a + 1;
				write(pfdL[1], &nL, sizeof(nL));
				for (i = a; i <= k; i++) {
					write(pfdL[1], x + i, sizeof(x[i]));
				}
			
				nR = b - k;
				write(pfdR[1], &nR, sizeof(nR));
				for (i = k + 1; i <= b; i++) {
					write(pfdR[1], x + i, sizeof(x[i]));
				}

				wait(NULL);
				wait(NULL);
				merge(x, a, k, b);
				
			}
		}
		//exit(0);
	}
	}
	else {
		close(pfdL[0]);
		close(pfdL[1]);
		close(pfdR[0]);
		close(pfdR[1]);
		// wait for the array to be sorted, then print
		wait(NULL);
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

