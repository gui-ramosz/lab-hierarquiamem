#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N, BLOCO;
double *A, *B, *C;

typedef struct {
    long linha_inicio;
    long linha_fim;
} ThreadArg;

void* thread_matmul_bloco(void* arg) {
    ThreadArg* targ = (ThreadArg*) arg;

    for (long ii = targ->linha_inicio; ii < targ->linha_fim; ii += BLOCO) {
        long i_lim = (ii + BLOCO < targ->linha_fim) ? ii + BLOCO : targ->linha_fim;
        for (long jj = 0; jj < N; jj += BLOCO) {
            long j_lim = (jj + BLOCO < N) ? jj + BLOCO : N;
            for (long kk = 0; kk < N; kk += BLOCO) {
                long k_lim = (kk + BLOCO < N) ? kk + BLOCO : N;
                for (long i = ii; i < i_lim; i++) {
                    for (long k = kk; k < k_lim; k++) {
                        double r = A[i * N + k];
                        for (long j = jj; j < j_lim; j++) {
                            C[i * N + j] += r * B[k * N + j];
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N> <num_threads> <B>\n", argv[0]);
        return 1;
    }
    N = atol(argv[1]);
    int NUM_THREADS = atoi(argv[2]);
    BLOCO = atol(argv[3]);

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
    long linhas_bloco = N / NUM_THREADS;

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].linha_inicio = t * linhas_bloco;
        args[t].linha_fim = (t == NUM_THREADS - 1) ? N : (t + 1) * linhas_bloco;
        pthread_create(&threads[t], NULL, thread_matmul_bloco, &args[t]);
    }
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / tempo / 1e9;

    printf("[MatmulPthreadsBloco] N=%ld | Threads=%d | Bloco=%ld | Tempo: %.4f s | GFLOPS: %.2f\n",
           N, NUM_THREADS, BLOCO, tempo, gflops);
    printf("Checksum (amostra C[0][0], C[N-1][N-1]): %.2f %.2f\n",
           C[0], C[(N - 1) * N + (N - 1)]);

    free(A); free(B); free(C);
    return 0;
}
