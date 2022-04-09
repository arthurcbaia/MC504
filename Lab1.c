#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define ROWS 100
#define COLUMNS 100

int matA[COLUMNS][ROWS], matB[COLUMNS][ROWS], matC[COLUMNS][ROWS] = {}, matD[COLUMNS][ROWS] = {};

void *multiply(void *param)
{
    int *parametros = (int *)param;
    for (int i = 0; i < ROWS; i++)
    {
        matC[parametros[0]][parametros[1]] += matA[parametros[0]][i] * matB[i][parametros[1]];
    }
}

void single_threaded_multiplication()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {

            for (int k = 0; k < ROWS; k++)
                matD[i][j] += matA[i][k] * matB[k][j];
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
            matC[i][j] = 0;
            matD[i][j] = 0;
        }
    }
    // printf("matriz A:\n");
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLUMNS; j++)
    //     {
    //         printf("%d ", matA[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("matriz B:\n");
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLUMNS; j++)
    //     {
    //         printf("%d ", matB[i][j]);
    //     }
    //     printf("\n");
    // }

    start = clock();
    single_threaded_multiplication();
    end = clock();
    printf("Resulting matrix from the single threaded function: \n");
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLUMNS; j++)
    //     {
    //         printf("%d ", matD[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("Time spent to multiplicate two matrices using single thread function %.2f\n", (double)(-start + end));

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
            pthread_create(&threads[i * COLUMNS + j], NULL, multiply, (void *)(p));
        }
    }
    end = clock();

    printf("Time spent to create the %d threads: %.2f\n", MAX_THREAD, (double)(-start + end));

    start = clock();
    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);
    end = clock();

    printf("Time spent to multiplicate matrices with %d rows and %d columns using %d threads: %.2f\n", ROWS, COLUMNS, MAX_THREAD, (double)(-start + end));

    // Displaying the result matrix
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLUMNS; j++)
    //         printf("%d ", matC[i][j]);
    //     printf("\n");
    // }
    return 0;
}