/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: A kliens információt kér állományokról. A szerver ellenőrzi, hogy létezik-e az adott állomány, és válaszként visszaküldi az
   állomány típusát, méretét, hány különböző neven hivatkoznak rá, szöveges állományok esetében pedig az állomány sorainak és szavainak számát is. */

#include "header.h"

int main(void) {
	int szf, klf, opts;
	char filename[MAXLEN];
	pid_t fk;

	if (mkfifo(SZFIFONAME, S_IFIFO|0666) < 0) {
		perror("error mkfifo\n");
		exit(1);
	}

	opts = O_RDONLY;
	opts &= ~O_NONBLOCK;
	if ((szf = open(SZFIFONAME, opts)) < 0) {
		perror("error opening szfifo\n");
		unlink(SZFIFONAME);
		exit(1);
	}

	while (1) {
		if (read(szf, filename, MAXLEN) > 0) {
			opts = O_WRONLY;
                        opts &= ~O_NONBLOCK;
			if (strcmp(filename, "exit") == 0) {
				printf("%d: server received 'exit' message\n", getpid());

				if ((klf = open(KLFIFONAME, opts)) < 0) {
                                        perror("error opening klfifo\n");
                                        exit(1);
                                }

                                write(klf, "server succesfully shut down", MAXLEN);
                                close(klf);

				close(szf);
				unlink(SZFIFONAME);
				exit(0);
			}

			if ((fk = fork()) < 0) {
				perror("error fork\n");
				exit(1);
			}
			if (fk == 0) {
				FILE *fp;
				char cmd[MAXLEN + 21];
				sprintf(cmd, "./L11_07_baam0146.sh %s", filename);

				if ((fp = popen(cmd, "r")) == NULL) {
					perror("error popen\n");
					exit(1);
				}

				fgets(cmd, MAXLEN, fp);
				cmd[strcspn(cmd, "\n")] = 0;
				printf("%d: received \"%s\", sent back \"%s\"\n", getpid(), filename, cmd);
				
				
				if ((klf = open(KLFIFONAME, opts)) < 0) {
					perror("error opening klfifo\n");
					unlink(SZFIFONAME);
					exit(1);
				}

				write(klf, cmd, MAXLEN);
				close(klf);

				exit(0);
			}
		}
		//else {
		//	perror("error reading from server fifo\n");
		//	exit(1);
		//}
	}

	close(szf);
	unlink(SZFIFONAME);

	return 0;
} 
