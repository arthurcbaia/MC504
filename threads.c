#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#define NUMBER_OF_THREADS 50
#define SIZE 50


int matA[SIZE][SIZE];
int matB[SIZE][SIZE];
int matC[SIZE][SIZE];
int step_i = 0;

void *f_thread(void *vargp)
{
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL; 
}

int main(){
  pthread_t t_id[NUMBER_OF_THREADS];

  for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matA[i][j] = rand() % 50;
            matB[i][j] = rand() % 50;
        }
    }

  for (int i = 0; i < NUMBER_OF_THREADS; i++)
  {
    pthread_create(&t_id[i], NULL, f_thread, NULL);
  }
  

}