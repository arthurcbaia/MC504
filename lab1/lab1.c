#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Número de linhas / colunas das matrizes (quadradas)
#define N 10

// Matrizes de entrada
// Preenchidas aleatoriamente na main
static int matA[N][N];
static int matB[N][N];

// Matriz de saída do multithread
static int matC[N][N] = {};

// Matriz de saída do single thread
static int matD[N][N] = {};

// Matriz de saída do multithread com thread pool
static int matE[N][N] = {};

void *multiply(void *param)
{
    int *parametros = (int *)param;
    for (int i = 0; i < N; i++)
        matC[parametros[0]][parametros[1]] += matA[parametros[0]][i] * matB[i][parametros[1]];
    free(parametros);
    return NULL;
}

void *threadPoolFunc(void *param)
{
    int *parametros = (int *)param;
    int i = parametros[0]; // i denotes row number of resultant matC

    for (int j = 0; j < N; j++)
        for (int k = 0; k < N; k++)
            matE[i][j] += matA[i][k] * matB[k][j];

    free(parametros);
    return NULL;
}

void single_threaded_multiplication()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                matD[i][j] += matA[i][k] * matB[k][j];
}

void printMat(int matX[N][N])
{
    printf("┌ ");
    for (int i = 0; i < N; i++)
        printf("    ");
    printf("┐\n");

    for (int i = 0; i < N; i++)
    {
        printf("│ ");
        for (int j = 0; j < N; j++)
            printf("%3d ", matX[i][j]);
        printf("│\n");
    }

    printf("└ ");
    for (int i = 0; i < N; i++)
        printf("    ");
    printf("┘\n");
}

int main()
{

    clock_t start, end;

    // Generating random values in matA and matB
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    printf("Mat A:\n");
    printMat(matA);
    printf("Mat B:\n");
    printMat(matB);

    printf("\n");
    printf("///////////////////////////////////////\n");
    printf("///         Single threaded         ///\n");
    printf("///////////////////////////////////////\n");
    printf("\n");

    start = clock();
    single_threaded_multiplication();
    end = clock();
    printf("Mat D:\n");
    printMat(matD);
    printf("Multiplication: %.2f ms\n\n", (double)(-start + end) * 1000.0 / CLOCKS_PER_SEC);

    printf("\n");
    printf("///////////////////////////////////////\n");
    printf("///           NxN threads           ///\n");
    printf("///////////////////////////////////////\n");
    printf("\n");

    pthread_t threads[N][N];
    start = clock();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int *p = (int *) malloc(sizeof(int) * 2);
            p[0] = i;
            p[1] = j;
            pthread_create(&threads[i][j], NULL, multiply, p);
        }
    }
    end = clock();
    printf("Create %d threads: %.2f ms\n", N * N,
        (double)(-start + end) * 1000.0 / CLOCKS_PER_SEC);

    // joining and waiting for all threads to complete
    start = clock();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            pthread_join(threads[i][j], NULL);
    end = clock();
    printf("Mat C:\n");
    printMat(matC);
    printf("Multiplication: %.2f ms\n", (double)(-start + end) * 1000.0 / CLOCKS_PER_SEC);

    printf("\n");
    printf("///////////////////////////////////////\n");
    printf("///            N threads            ///\n");
    printf("///////////////////////////////////////\n");
    printf("\n");

    pthread_t pool[N];
    start = clock();
    for (int i = 0; i < N; i++)
    {
        int *x = (int *) malloc(sizeof(int));
        x[0] = i;
        pthread_create(&pool[i], NULL, threadPoolFunc, (void *)(x));
    }
    end = clock();
    printf("Create %d threads: %.2f ms\n", N,
        (double)(-start + end) * 1000.0 / CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < N; t++)
        pthread_join(pool[t], NULL);
    end = clock();
    printf("Mat E:\n");
    printMat(matE);
    printf("Multiplication: %.2f ms\n\n", (double)(-start + end) * 1000.0 / CLOCKS_PER_SEC);

    return 0;
}