/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: A kliens információt kér állományokról. A szerver ellenőrzi, hogy létezik-e az adott állomány, és válaszként visszaküldi az
   állomány típusát, méretét, hány különböző neven hivatkoznak rá, szöveges állományok esetében pedig az állomány sorainak és szavainak számát is. */

#include "header.h"

int main(void) {
	int szf, klf;
	char filename[MAXLEN];
	pid_t fk;

	if (mkfifo(SZFIFONAME, S_IFIFO|0666) < 0) {
		perror("error mkfifo\n");
		exit(1);
	}

	if ((szf = open(SZFIFONAME, O_RDONLY)) < 0) {
		perror("error opening szfifo\n");
		exit(1);
	}

	while (1) {
		if (read(szf, filename, MAXLEN) > 0) {
			if (strcmp(filename, "exit") == 0) {
				printf("%d: server received 'exit' message\n", getpid());
				close(szf);
				unlink(SZFIFONAME);
				// ide asszem kell egy wait(NULL);
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
				
				
				if ((klf = open(KLFIFONAME, O_WRONLY)) < 0) {
					perror("error opening klfifo\n");
					exit(1);
				}

				write(klf, cmd, MAXLEN);
				close(klf);

				exit(0);
			}
		}
		else {
			perror("error reading from server fifo\n");
			exit(1);
		}
	}

	close(szf);
	unlink(SZFIFONAME);

	

















	/*
	FILE* testing;
	char res[1024], cmd[1024];

	strcpy(cmd, "./script.sh ");
	strcat(cmd, "\"hellooo gorgeous\"");
	testing = popen(cmd, "r");

	fgets(res, 1024, testing);
	printf("%s\n", res);

	pclose(testing);
	*/

	return 0;
} 
