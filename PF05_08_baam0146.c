#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 2048

int main(void) {
	char evoeszk[15];
	FILE *fin;
	int annanak[2], beanak[2], csongornak[2];
	pid_t gy1, gy2, gy3;
	int status;
	

	if (pipe(annanak) < 0)
		perror("pipe hiba\n");
	if (pipe(beanak) < 0)
		perror("pipe hiba\n");
	if (pipe(csongornak) < 0)
		perror("pipe hiba\n");

	if ((gy1 = fork()) < 0)
		perror("fork hiba\n");

	if (gy1 != 0) { // ha Anya, akkor gyart meg 2 gyereket
		if ((gy2 = fork()) < 0)
			perror("fork hiba\n");

		if (gy2 != 0) { // ha Anya, akkor meg gyart 1 gyereket
			if ((gy3 = fork()) < 0)
				perror("fork hiba\n");

			if (gy3 != 0) { // ha Anya, akkor feldolgozza a bemenetet es kuldi gyerekeknek
				fin = fopen("input.dat", "r");

				while (fscanf(fin, "%s", evoeszk) == 1) {
					if (strcmp(evoeszk, "kes") == 0 || strcmp(evoeszk, "kanal") == 0 || strcmp(evoeszk, "villa") == 0)
						write(annanak[1], evoeszk, sizeof(evoeszk));
					else
						write(beanak[1], evoeszk, sizeof(evoeszk));
				}
				strcpy(evoeszk, "exit");
				write(annanak[1], evoeszk, sizeof(evoeszk));
				write(beanak[1], evoeszk, sizeof(evoeszk));
				waitpid(gy1, &status, WUNTRACED);
				waitpid(gy2, &status, WUNTRACED);
				write(csongornak[1], evoeszk, sizeof(evoeszk)); // ez lehet nem felel meg a feladlat keresenek
				waitpid(gy3, &status, WNOHANG);

				fclose(fin);
			}
			else { // Csongor
				while (1) {
					read(csongornak[0], evoeszk, sizeof(evoeszk));
					if (strcmp(evoeszk, "exit") == 0)
						exit(0);
					printf("Csongor helyretesz %s\n", evoeszk);
				}
			}
		}
		else { // Bea
			while (1) {
				read(beanak[0], evoeszk, sizeof(evoeszk));
				if (strcmp(evoeszk, "exit") == 0)
					exit(0);
				printf("Bea mosogat %s\n", evoeszk);
				write(csongornak[1], evoeszk, sizeof(evoeszk));
			}
		}
	}
	else { // Anna
		while (1) {
			read(annanak[0], evoeszk, sizeof(evoeszk));
			if (strcmp(evoeszk, "exit") == 0)
				exit(0);
			printf("Anna mosogat %s\n", evoeszk);
			write(csongornak[1], evoeszk, sizeof(evoeszk));
		}
	}

	return 0;
}
