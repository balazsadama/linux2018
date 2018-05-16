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
	int k, unsorted = 1;
	int pfdL[2], nL, pfdR[2], nR;
	char buf[MAXLINE];

	// open pipe for left child
	pipe(pfdL);
	// open pipe for right child
	pipe(pfdR);

	root = fork();

	if (root == 0) {
		// we create 2 child processes to sort left and right part of array
		child1 = fork();

		if (child1 == 0) { // if child, then sort left
			/*nL = read(pfdL[0], buf, MAXLINE);
			printf("%d child read ", getpid());
			fflush(stdout);
			write(1, buf, nL);
			write(1, "\n", 1);
			write(pfdL[1], "69", 2);*/

			read(pfdL[0], &nL, sizeof(nL));
			int i, x;
			for (i = 0; i < nL; i++) {
				read(pfdL[0], &x, sizeof(x));
				printf("%d ", x);
			}
		}
		else { 
			
			
			
			// else send input to child
			/*
			char test[] = "sending this from parent";
			write(pfdL[1], test, sizeof(test));
			wait(NULL);
			nL = read(pfdL[0], buf, MAXLINE);
			printf("%d parent read ", getpid());
			fflush(stdout);
			write(1, buf, nL);
			write(1, "\n", 1); */

			int i = 7;
			write(pfdL[1], &i, sizeof(i));
			for (i = 0; i < 7; i++) {
				write(pfdL[1], &i, sizeof(i));
			}
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
		for (int i = a; i < b; i++)
	                printf("%d ", x[i]);
		exit(1);
	}
}


int main(void){
//	int t[6] = {1, 3, 5, 2, 4, 6};
	int t[5] = {1, 5, 3, 4, 2};

/*	for (int i = 0; i < 5; i++) 
		printf("%d ", t[i]);
*/
	mergeSort(t, 0, 4);

	/*
	printf("\n");
	for (int i = 0; i < 5; i++) 
                printf("%d ", t[i]);
*/
	return 0;
}

