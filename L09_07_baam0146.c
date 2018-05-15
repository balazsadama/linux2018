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
	pid_t child1 = -1, child2 = -1;
	int k, unsorted = 1;


	// change this if to while
	while (unsorted) {
		child1 = fork();
		k = a + (b - a) / 2;

		// child1 sorts left part of array
		if (child1 == 0) {
			b = k;
			printf("I am child1, my pid is %d and my parent's pid is %d.\n", getpid(), getppid());
			if (a == b)
				exit(1);
			// else backtrack
		}
		else {
			child2 = fork();
			// child2 sorts right part of array
			if (child2 == 0) {
				a = k + 1;
				printf("I am child2, my pid is %d and my parent's pid is %d.\n", getpid(), getppid());
				if (a == b)
					exit(1);
				// else backtrack
			}
			else {
				wait(NULL);
				wait(NULL);
				printf("I am the parent of %d and %d and my pid is %d.\n", child1, child2, getpid());
				merge(x, a, k, b);
				exit(1);
			}
		}
	}
}

/*
int main(void){
	int t[6] = {1, 3, 5, 2, 4, 6};

	for (int i = 0; i < 5; i++) 
		printf("%d ", t[i]);

	mergeSort(t, 0, 5);

	printf("\n");
	for (int i = 0; i < 5; i++) 
                printf("%d ", t[i]);

	return 0;
}
*/

/*
 * while ( a != b) {
 * 	k = n / 2;
 *	b = k;
 *	fork();
 *	a = k;
 *	fork();
 *
 * } */
