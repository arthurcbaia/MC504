#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define ROWS 20
#define COLUMNS 20

int matA[COLUMNS][ROWS], matB[COLUMNS][ROWS], matC[COLUMNS][ROWS] = {};

void *multiply(void *param)
{
    int* humor = (int*)param;
    printf("%d %d\n",humor[0], humor[1]);
}

void single_threaded_multiplication()
{
    printf("multiply of the matrix=\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            
        }
    }
}

int main()
{

    clock_t start, end;
    double cpu_time_used;

    // Generating random values in matA and matB
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    int MAX_THREAD = ROWS * COLUMNS;

    pthread_t threads[MAX_THREAD];

    start = clock();

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            int *p;
            p = malloc(sizeof(int) * 2);
            p[0] = i;
            p[1] = j;
            pthread_create(&threads[i*COLUMNS + j], NULL, multiply, (void *)(p));
        }
    }
    end = clock();

    printf("Time spent to create the %d threads: %.2f\n", MAX_THREAD, (double)(-start + end));

    start = clock();
    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);
    end = clock();

    printf("Time spent to multiplicate matrixes with %d rows and %d columns using %d threads: %.2f\n", ROWS, COLUMNS, MAX_THREAD, (double)(-start + end));

    // // Displaying the result matrix
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLUMNS; j++)
    //         printf("%d ", matC[i][j]);
    //     printf("\n");
    // }
    return 0;
}