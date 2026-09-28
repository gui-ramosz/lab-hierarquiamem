#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N;
double *A, *B, *C;

typedef struct {
    long linha_inicio;
    long linha_fim;
} ThreadArg;

void* thread_matmul(void* arg) {
    ThreadArg* targ = (ThreadArg*) arg;
    for (long i = targ->linha_inicio; i < targ->linha_fim; i++) {
        for (long j = 0; j < N; j++) {
            double soma = 0.0;
            for (long k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }
    N = atol(argv[1]);
    int NUM_THREADS = atoi(argv[2]);

    A = malloc((size_t) N * N * sizeof(double));
    B = malloc((size_t) N * N * sizeof(double));
    C = calloc((size_t) N * N, sizeof(double));
    if (!A || !B || !C) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        return 1;
    }

    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            A[i * N + j] = (double) (i + j);
            B[i * N + j] = (double) (i * j);
        }
    }

    pthread_t threads[NUM_THREADS];
    ThreadArg args[NUM_THREADS];
    long bloco = N / NUM_THREADS;

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].linha_inicio = t * bloco;
        args[t].linha_fim = (t == NUM_THREADS - 1) ? N : (t + 1) * bloco;
        pthread_create(&threads[t], NULL, thread_matmul, &args[t]);
    }
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / tempo / 1e9;

    printf("[MatmulPthreads] N=%ld | Threads=%d | Tempo: %.4f s | GFLOPS: %.2f\n",
           N, NUM_THREADS, tempo, gflops);
    printf("Checksum (amostra C[0][0], C[N-1][N-1]): %.2f %.2f\n",
           C[0], C[(N - 1) * N + (N - 1)]);

    free(A); free(B); free(C);
    return 0;
}
