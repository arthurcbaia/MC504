#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// Resolveremos o problema "dining savages problem"
/// https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf p 115

int numSavages = 0;
int maxServings = 0;

// Apenas para impressão na tela

const char **savagesStatus;
const char *cookStatus = "...";

//

int numServingsRemaining = 0;
sem_t mutex;
sem_t semEmptyPot;
sem_t semFullPot;

void *savageMain(void *parameter)
{
    size_t i = (size_t)parameter;

    while (1) {
        savagesStatus[i] = "queued to get serving";
        sem_wait(&mutex);

        if (numServingsRemaining == 0) {
            savagesStatus[i] = "waking cook";
            sleep(1);
            sem_post(&semEmptyPot);

            savagesStatus[i] = "waiting";
            sem_wait(&semFullPot);
        }

        savagesStatus[i] = "get serving";
        sleep(1);

        numServingsRemaining -= 1;
        sem_post(&mutex);

        savagesStatus[i] = "eating";

        sleep(rand() % 7 + 1);
    }
}

void *cookMain(void *parameter)
{
    while (1) {
        cookStatus = "waiting";
        sem_wait(&semEmptyPot);

        cookStatus = "filling pot";
        sleep(5);
        numServingsRemaining = maxServings;

        sem_post(&semFullPot);
    }
}

// Funções de auxilio para interface

int shouldStop = 0;
void sigint(int _)
{
    shouldStop = 1;
}

int main(int argc, char **argv)
{
    if (argc == 3) {
        numSavages = atoi(argv[1]);
        maxServings = atoi(argv[2]);
    }

    if (numSavages == 0 || maxServings == 0) {
        printf("Uso: lab2 <numero de selvagens> <numero de servidas>\n");
        return 0;
    }

    // Inicialização

    numServingsRemaining = maxServings;
    if (sem_init(&mutex, 0, 1)) {
        return 1;
    }
    if (sem_init(&semEmptyPot, 0, 0)) {
        return 1;
    }
    if (sem_init(&semFullPot, 0, 0)) {
        return 1;
    }

    savagesStatus = malloc(sizeof(const char *) * numSavages);
    if (!savagesStatus) {
        return 1;
    }

    pthread_t savages[numSavages];
    for (size_t i = 0; i < numSavages; i++) {
        savagesStatus[i] = "...";
        if (pthread_create(&savages[i], NULL, savageMain, (void *)i)) {
            return 1;
        }
    }

    pthread_t cook;
    if (pthread_create(&cook, NULL, cookMain, NULL)) {
        return 1;
    }

    // Interface

    const char **loadedStatus = malloc(sizeof(const char *) * numSavages);
    if (!loadedStatus) {
        return 1;
    }

    memset((void *)loadedStatus, 0, numSavages);

    signal(SIGINT, sigint);
    printf("\e[1;1H\e[2J"); // Limpa o console
    printf("\e[?25l");      // Esconde o cursor
    while (!shouldStop) {
        printf("\033[0;0f"); // Move o cursor para linha 0 coluna 0
        printf("\33[2K");    // Limpa a linha
        printf("Servings: %02d / %02d\n", numServingsRemaining, maxServings);
        printf("\33[2K"); // Limpa a linha
        printf("Cook:     %s", cookStatus);

        for (int i = 0; i < numSavages; i++) {
            if (savagesStatus[i] != loadedStatus[i]) {
                loadedStatus[i] = savagesStatus[i];
                printf("\033[%d;0f",
                       i + 5);    // Move o cursor para linha i + 5 coluna 0
                printf("\33[2K"); // Limpa a linha
                printf("Savage %02d: %s", i, loadedStatus[i]);
            }
        }

        usleep(100);
    }
    printf("\e[1;1H\e[2J"); // Limpa o console
    printf("\e[?25h");      // Mostra o cursor

    return 0;
}
