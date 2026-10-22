#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {

    int pipe1[2]; // A fino B
    int pipe2[2]; // B fino C

    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(-1);
    }

    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(-2);
    }

    pid_t ritornoForkB = fork();
    if (ritornoForkB == -1) {
        perror("forkB");
        exit(-3);
    }

    if (!ritornoForkB) {
        //figlio (B)
        close(pipe1[1]);
        close(pipe2[0]);

        int n;

        while (read(pipe1[0], &n, sizeof(n)) > 0) {
            if (n >= 50) {
                int quad = n*n;
                write(pipe2[1], &quad, sizeof(quad));
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);

    }

    pid_t ritornoForkC = fork();
    if (ritornoForkC == -1) {
        perror("fork C");
        exit(-4);
    }

    if (!ritornoForkC) {
        // secondo figlio di a (C)
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        int n;
        int somma = 0;
        int tot = 0;

        while (read(pipe2[0], &n, sizeof(n)) > 0) {
            printf("(C) Ricevuto: %d\n", n);
            somma += n;
            tot++;
        }

        close(pipe2[0]);

        if (tot > 0) {
            double media = (double)somma/tot;
            printf("(C) Media: %.2f\n", media);
        } else {
            printf("(C) Nessun numero ricevuto\n");
        }
        exit(0);
    }

    // processo A (padre)

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);

    srand(time(NULL)); //rand veramente casuale (seme diverso ad ogni eseccuzione perchè time(NULL)
                        //ritorna i secondi dal 01/01/1970
    for (int i=0; i<20; i++) {
        int n = rand() % 100 + 1; //da 1 a 100
        printf("(A) Generato: %d\n", n);
        fflush(stdout);
        write(pipe1[1], &n, sizeof(n));
    }

    close (pipe1[1]);

    wait(NULL);
    wait(NULL);
    return 0;
}