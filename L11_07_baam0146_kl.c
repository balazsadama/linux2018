/* nev: Balazs Adam-Attila
   felhasznalo: baam0146
   feladat: A kliens információt kér állományokról. A szerver ellenőrzi, hogy létezik-e az adott állomány, és válaszként visszaküldi az
   állomány típusát, méretét, hány különböző neven hivatkoznak rá, szöveges állományok esetében pedig az állomány sorainak és szavainak számát is. */

#include "header.h"

int main(int argc, char **argv) {
	int szf, klf, opts;
        char result[MAXLEN];
	//pid_t fk;

	if (argc < 2) {
		perror("Please give a filename as parameter\n");
		exit(1);
	}

	// ha nem letezik, akkor letrehozzuk
	if(access(KLFIFONAME, F_OK) == -1) {
		if (mkfifo(KLFIFONAME, S_IFIFO|0666) < 0) {
			perror("error mkfifo\n");
       	        	exit(1);
		}
	}

	opts = O_WRONLY;
	opts &= ~O_NONBLOCK;
	//if ((szf = open(SZFIFONAME, O_WRONLY | O_NONBLOCK)) < 0) {
	if ((szf = open(SZFIFONAME, opts)) < 0) {
		perror("error opening szfifo\n");
		unlink(KLFIFONAME);
                exit(1);
	}


	int i = 1;
	while (i < argc) {
		/*if ((fk = fork()) < 0) {
			perror("error fork\n");
			exit(1);
		}*/
		//if (fk == 0) {
	
			printf("client trying to send %s\n", argv[i]);	
			if (write(szf, argv[i], MAXLEN) > 0) {
				printf("client wrote %s\n", argv[i]);
				//if ((klf = open(KLFIFONAME, O_RDONLY | (~O_NONBLOCK))) < 0) {
				opts = O_RDONLY;
				opts &= ~O_NONBLOCK;
				if ((klf = open(KLFIFONAME, opts)) < 0) {
					perror("error opening klfifo\n");
					exit(1);
				}

				if (read(klf, result, MAXLEN) < 0) {
					perror("error reading from klient fifo\n");
					exit(1);
				}
			
				printf("%d: sent \"%s\" to server and received \"%s\"\n", getpid(), argv[i], result);
				close(klf);

				// if "exit" received, then we close klfifo
				if (strcmp(argv[i], "exit") == 0) {
                        		unlink(KLFIFONAME);
                		}
			}
			else {
				perror("error writing to server fifo\n");
				exit(1);
			}
			//exit(0);
		//}
		i++;
	}

	//write(szf, "exit", sizeof("exit"));
	//close(szf);
	//unlink(KLFIFONAME);

	return 0;
}
