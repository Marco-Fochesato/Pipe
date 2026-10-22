#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    int arr[10] = {3, 8, 15, 4, 22, 7, 10,9,1};
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(-1);
    }

    pid_t ritorno_fork = fork();
    if (ritorno_fork == -1) {
        perror("fork");
        exit(-2);
    }

    if (!ritorno_fork) {
        //Chiudiamo il file descriptor che non usiamo
        close(fd[0]);
        int sommaPari = 0;
        for (int i = 0; i < 10; i++) {
            if (arr[i] %2 == 0) {
                sommaPari += arr[i];
            }
        }
        write(fd[1],  &sommaPari, sizeof(sommaPari));

        close(fd[1]);
        exit(0);
    }

    close(fd[1]);

    int sommaDispari = 0;
    for (int i = 0; i < 10; i++){
        if (arr[i] %2 != 0) {
            sommaDispari += arr[i];
        }
    }

    int sommaPari = read(fd[0], &sommaPari, sizeof(sommaPari));
    close(fd[0]);

    wait(NULL);

    printf("Somma dei dati pari (figlio): %d\n", sommaPari);
    printf("Somma dei dati dispari (padre): %d\n", sommaDispari);
    printf("Somma totale: %d", sommaPari+sommaDispari);
}