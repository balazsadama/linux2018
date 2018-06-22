/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: Írjunk olyan programot, amely egy n2 db. egész számot tartalmazó állománnyal dolgozik. Az állomány bejegyzései egy n elemű M
   négyzetes mátrix elemei. Egy program ismételten kiszámolja egy sor vagy egy oszlop elemeinek összegét (a sor vagy oszlopszámot véletlenszerűen
   generáljuk). Az összeget a sor vagy oszlop utolsó elemében tároljuk. Mikor egy számot felhasználtunk az összeg kiszámításánál lenullázzuk azt,
   elkerülve így azt, hogy kétszer is összeszámoljuk. Számoljuk ki M[n,n]-ben a mátrix elemeinek összegét több ilyen program párhuzamos indításával.
   Azt a részt az állományból, amit a program az összegszámolásnál használ mindig zárolni kell. A folyamatok akkor állnak le, amikor kiszámoltuk a
   végső összeget. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int done(char* filename) {
        int fd = open(filename, O_RDONLY);
        int n, x, i, j;

        read(fd, &n, sizeof(n));
        for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                        if (read(fd, &x, sizeof(x)) < 0) {
                                perror("error reading\n");
                                close(fd);
                                exit(1);
                        }
			if (!(i == n - 1 && j == n - 1) && x != 0)
				return 0;
                }
        }

        close(fd);
        return 1;
}

int main(int argc, char **argv) {
	int fd, n, i, x, k, sum;
	FILE* fout;
	struct flock lockS;
	srand(time(NULL));

	if (argc < 2) {
		printf("Please give input filename as first parameter.\n");
		exit(1);
	}

	if ((fd = open(argv[1], O_RDWR, 0644)) < 0) {
		fprintf(stderr, "%d: error opening file %s\n", getpid(), argv[1]);
		exit(1);
	}

	if (read(fd, &n, sizeof(n)) < 0) {
		fprintf(stderr, "%d: error reading from fd\n", getpid());
		exit(1);
	}
	printf("%d read n=%d from fd\n", getpid(), n);


	memset(&lockS, 0, sizeof(struct flock));
        lockS.l_whence = SEEK_CUR;
        lockS.l_start = 0;
	while (!done(argv[1])) {
		if (rand() % 2) {
			// decide what row to process
			k = rand() % n;
			printf("%d: random decided: row %d\n", getpid(), k);
			// lock the row
			lockS.l_type = F_WRLCK;
			lockS.l_len = n * sizeof(int);
			lseek(fd, sizeof(int) + k * n * sizeof(int), SEEK_SET);
			if (fcntl(fd, F_SETLKW, &lockS) < 0) {
				fprintf(stderr, "%d: error when trying to lock\n", getpid());
				exit(1);
			}
			printf("%d: locked the row %d\n", getpid(), k);

			// process the row
			sum = 0;
			for (i = 0; i < n; i++) {
				if (read(fd, &x, sizeof(x)) < 0) {
		                        fprintf(stderr, "%d: error reading from fd\n", getpid());
		                        exit(1);
		                }
		                sum += x;
		                x = 0;
				lseek(fd, (-1) * sizeof(int), SEEK_CUR);
		                if (i == n - 1) {
		                        if (write(fd, &sum, sizeof(sum)) < 0) {
		                                fprintf(stderr, "%d: error writing to fd\n", getpid());
		                                exit(1);
		                        }
		                }
		                else {
		                        if (write(fd, &x, sizeof(x)) < 0) {
		                                fprintf(stderr, "%d: error writing to fd\n", getpid());
		                                exit(1);
		                        }
		                }
			}
			// unlock the row
			lseek(fd, (-1) * n * sizeof(int), SEEK_CUR);
			lockS.l_type = F_UNLCK;
			if (fcntl(fd, F_SETLKW, &lockS) < 0) {
		                fprintf(stderr, "%d: error when trying to unlock\n", getpid());
		                exit(1);
		        }
			printf("%d: unlocked the row %d\n", getpid(), k);
		}
		else {
			// decide what column to process
			k = rand() % n;
			printf("%d: random decided: col %d\n", getpid(), k);
			// lock the column
			lockS.l_type = F_WRLCK;
			lockS.l_len = sizeof(int);
			for (i = 0; i < n; i++) {
			        lseek(fd, sizeof(int) + i * n * sizeof(int) + k * sizeof(int), SEEK_SET);
			        if (fcntl(fd, F_SETLKW, &lockS) < 0) {
			                fprintf(stderr, "%d: error when trying to lock\n", getpid());
			                exit(1);
			        }
			}
			printf("%d: locked the column %d\n", getpid(), k);

			// process the column
			sum = 0;
			for (i = 0; i < n; i++) {
			        lseek(fd, sizeof(int) + i * n * sizeof(int) + k * sizeof(int), SEEK_SET);
			        if (read(fd, &x, sizeof(x)) < 0) {
			                fprintf(stderr, "%d: error reading from fd\n", getpid());
			                exit(1);
			        }
			        sum += x;
			        x = 0;
			        lseek(fd, (-1) * sizeof(int), SEEK_CUR);
			        if (i == n - 1) {
			                if (write(fd, &sum, sizeof(sum)) < 0) {
			                        fprintf(stderr, "%d: error writing to fd\n", getpid());
			                        exit(1);
			                }
			        }
			        else {
			                if (write(fd, &x, sizeof(x)) < 0) {
			                        fprintf(stderr, "%d: error writing to fd\n", getpid());
			                        exit(1);
			                }
			        }
			}

			// unlock the column
			lockS.l_type = F_UNLCK;
			for (i = 0; i < n; i++) {
			        lseek(fd, sizeof(int) + i * n * sizeof(int) + k * sizeof(int), SEEK_SET);
			        if (fcntl(fd, F_SETLKW, &lockS) < 0) {
			                fprintf(stderr, "%d: error when trying to unlock\n", getpid());
			                exit(1);
			        }
			}
			printf("%d: unlocked column %d\n", getpid(), k);
		}
	}

	if ((fout = fopen("output.dat", "w")) < 0) {
		perror("error opening output file\n");
		exit(1);
	}

	lockS.l_type = F_RDLCK;
	lockS.l_len = sizeof(int);
	lseek(fd, n * n * sizeof(int), SEEK_SET);
	if (fcntl(fd, F_SETLK, &lockS) < 0) {
		fprintf(stderr, "%d: error when trying to lock\n", getpid());
		exit(1);
	}
	lseek(fd, n * n * sizeof(int), SEEK_SET);
	if (read(fd, &x, sizeof(x)) < 0) {
		fprintf(stderr, "%d: error reading from fd\n", getpid());
		exit(1);
	}

	lockS.l_type = F_UNLCK;
	lseek(fd, n * n * sizeof(int), SEEK_SET);
	if (fcntl(fd, F_SETLK, &lockS) < 0) {
                fprintf(stderr, "%d: error when trying to unlock\n", getpid());
                exit(1);
        }
	fprintf(fout, "%d", x);
	if ((fclose(fout)) < 0) {
		fprintf(stderr, "%d: error when trying to close output file\n", getpid());
                exit(1);
        }

	printf("%d ended, result is %d\n", getpid(), x);
	close(fd);
	return 0;
}
