#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define ROWS 10
#define COLUMNS 10
#define THREAD_POOL ROWS

int matA[COLUMNS][ROWS], matB[COLUMNS][ROWS], matC[COLUMNS][ROWS] = {}, matD[COLUMNS][ROWS] = {}, matE[COLUMNS][ROWS] = {}, linha_atual;

void *multiply(void *param)
{
    int *parametros = (int *)param;
    for (int i = 0; i < ROWS; i++)
    {
        matC[parametros[0]][parametros[1]] += matA[parametros[0]][i] * matB[i][parametros[1]];
    }
}

void *threadPoolFunc(void *param)
{

    int *parametros = (int *)param;
    int i = linha_atual++; // i denotes row number of resultant matC

    for (int j = 0; j < THREAD_POOL; j++)
        for (int k = 0; k < THREAD_POOL; k++)
            matE[i][j] += matA[i][k] * matB[k][j];
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

void print(int matX[COLUMNS][ROWS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            printf("%d ", matX[i][j]);
        }
        printf("\n");
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
            matE[i][j] = 0;
        }
    }

    start = clock();
    single_threaded_multiplication();
    end = clock();
    printf("Time spent to multiplicate two matrices using single thread function %.2f\n", (double)(-start + end));

    int MAX_THREAD = ROWS * COLUMNS;

    pthread_t threads[MAX_THREAD];
    pthread_t pool[THREAD_POOL];

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

    if (!(MAX_THREAD % THREAD_POOL) && ROWS == THREAD_POOL)
    {
        start = clock();
        for (int i = 0; i < THREAD_POOL; i++)
        {
            int *x;
            pthread_create(&pool[i], NULL, threadPoolFunc, (void *)(x));
        }

        end = clock();
        printf("Time spent to create the %d threads: %.2f\n", THREAD_POOL, (double)(-start + end));

        start = clock();
        for (int t = 0; t < THREAD_POOL; t++)
            pthread_join(pool[t], NULL);
        end = clock();
        printf("Time spent to multiplicate matrices with %d rows and %d columns using %d threads: %.2f\n", ROWS, COLUMNS, THREAD_POOL, (double)(-start + end));
    }

    printf("Single threaded result:\n");
    print(matD);

    printf("NxN threads result:\n");
    print(matC);

    printf("N threads result:\n");
    print(matE);

    return 0;
}