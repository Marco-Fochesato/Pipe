#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct Messaggio {
    char testo[256];
    int vocali;
};
int main(void) {
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(-1);
    }

    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(-2);
    }

    pid_t ritornoFork = fork();

    if (ritornoFork == -1) {
        perror("fork");
        exit(-3);
    }

    if (!ritornoFork) {

        close(pipe1[1]);
        close(pipe2[0]);

        char buffer[256];
        read(pipe1[0], buffer, sizeof(buffer));

        struct Messaggio msg;
        msg.vocali = 0;

        int i;
        for (i=0; buffer[i] != '\0'; i++) {
            if (buffer[i] == 'A' || buffer[i] == 'E' || buffer[i] == 'I' || buffer[i] == 'O' || buffer[i] == 'U' ||
                buffer[i] == 'a' || buffer[i] == 'e' || buffer[i] == 'i' || buffer[i] == 'o' || buffer[i] == 'u') {
                msg.vocali++;
            }

            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                msg.testo[i] = buffer[i] - ('a' - 'A');
            } else {
                msg.testo[i] = buffer[i];
            }


        }
        msg.testo[i] = '\0';

        write(pipe2[1], &msg, sizeof(msg));

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    close(pipe1[0]);
    close(pipe2[1]);

    char input[256];
    printf("Inserisci una stringa: ");
    scanf("%255[^\n]", input); //legge tutto fino al newline e aggiunge \0, 255 limite per non scrivere fuori dall'array

    int lung = 0;
    while (input[lung] != '\0') {
        lung++;
    }

    write(pipe1[1], input, lung+1);
    close(pipe1[1]);

    struct Messaggio risposta;
    read(pipe2[0], &risposta, sizeof(risposta));
    close(pipe2[0]);

    wait(NULL);

    printf("Stringa maiuscola: %s\n", risposta.testo);
    printf("Numero vocali: %d\n", risposta.vocali);
    return 0;
}