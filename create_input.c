#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(void) {
	int x, i, j, n, fd;
	FILE *szoveges;

	srand(time(NULL));
	n = rand() % 6;
	printf("n = %d\n", n);

	if ((fd = open("input.dat", O_WRONLY|O_CREAT, 0666)) < 0) {
		perror("error: open\n");
		exit(1);
	}
	if ((szoveges = fopen("szoveges_input", "w")) == NULL) {
		perror("error: fopen\n");
		exit(1);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			x = rand() % 100;
			write(fd, &x, sizeof(int));
			fprintf(szoveges, "%d ", x);
		}
		fprintf(szoveges, "\n");
	}

	fclose(szoveges);
	close(fd);

	if ((fd = open("input.dat", O_RDONLY)) < 0) {
		perror("error: popen\n");
		exit(1);
	}

	for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
			read(fd, &x, sizeof(x));
			printf("%d ", x);
		}
		printf("\n");
	}
		
	close(fd);
	return 0;
}
