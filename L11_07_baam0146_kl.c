/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: A kliens információt kér állományokról. A szerver ellenőrzi, hogy létezik-e az adott állomány, és válaszként visszaküldi az
   állomány típusát, méretét, hány különböző neven hivatkoznak rá, szöveges állományok esetében pedig az állomány sorainak és szavainak számát is. */

#include "header.h"

int main(void) {
	FILE *fin;
	int szf, klf;
        char filename[MAXLEN], result[MAXLEN];

	if (mkfifo(KLFIFONAME, S_IFIFO|0666) < 0) {
		perror("error mkfifo\n");
                exit(1);
	}

	if ((szf = open(SZFIFONAME, O_WRONLY)) < 0) {
		perror("error opening szfifo\n");
                exit(1);
	}

	if ((fin = fopen("input.dat", "r")) < 0) {
		perror("error opening input.dat\n");
		exit(1);
	}

	while (fscanf(fin, "%s", filename) != EOF) {
		if (write(szf, filename, MAXLEN) > 0) {
			//printf("%d: sent %s to server and received ", getpid(), filename);

			if ((klf = open(KLFIFONAME, O_RDONLY)) < 0) {
				perror("error opening klfifo\n");
				exit(1);
			}


			//printf("KLIENSBEN KLF = %d\n", klf);


			//printf("%d: client SUCCESFULLY OPENED client fifo for read\n", getpid());
			
			/*char c;
			while(read(klf, &c, sizeof(char)) > 0)
				printf("%c", c);
			printf("\n");
			*/
			
			if (read(klf, result, MAXLEN) < 0) {
				perror("error reading from klient fifo\n");
				exit(1);
			}
			
			printf("%d: sent %s to server and received \"%s\"\n", getpid(), filename, result);
			//printf("\"%s\"\n", result);
			close(klf);
		}
		else {
			perror("error writing to server fifo\n");
			exit(1);
		}
	}

	write(szf, "exit", sizeof("exit"));
	close(szf);
	unlink(KLFIFONAME);

	fclose(fin);
	return 0;
}
