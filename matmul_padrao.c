#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    long N = atol(argv[1]);

    double* A = malloc((size_t) N * N * sizeof(double));
    double* B = malloc((size_t) N * N * sizeof(double));
    double* C = calloc((size_t) N * N, sizeof(double));
    if (!A || !B || !C) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        return 1;
    }

    /* Inicializacao deterministica padronizada */
    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            A[i * N + j] = (double) (i + j);
            B[i * N + j] = (double) (i * j);
        }
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            double soma = 0.0;
            for (long k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / tempo / 1e9;

    printf("[Matmul Padrao] N: %ld | Tempo: %.4f s | GFLOPS: %.2f\n", N, tempo, gflops);
    printf("Checksum (amostra C[0][0], C[N-1][N-1]): %.2f %.2f\n",
           C[0], C[(N - 1) * N + (N - 1)]);

    free(A); free(B); free(C);
    return 0;
}
