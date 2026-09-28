#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <B>\n", argv[0]);
        return 1;
    }
    long N = atol(argv[1]);
    long B = atol(argv[2]);

    double* A = malloc((size_t) N * N * sizeof(double));
    double* Bm = malloc((size_t) N * N * sizeof(double));
    double* C = calloc((size_t) N * N, sizeof(double));
    if (!A || !Bm || !C) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        return 1;
    }

    /* Inicializacao deterministica padronizada (identica a matmul_padrao.c) */
    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            A[i * N + j] = (double) (i + j);
            Bm[i * N + j] = (double) (i * j);
        }
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long ii = 0; ii < N; ii += B) {
        for (long jj = 0; jj < N; jj += B) {
            for (long kk = 0; kk < N; kk += B) {
                long i_max = (ii + B < N) ? ii + B : N;
                long k_max = (kk + B < N) ? kk + B : N;
                long j_max = (jj + B < N) ? jj + B : N;
                for (long i = ii; i < i_max; i++) {
                    for (long k = kk; k < k_max; k++) {
                        double r = A[i * N + k];
                        for (long j = jj; j < j_max; j++) {
                            C[i * N + j] += r * Bm[k * N + j];
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / tempo / 1e9;

    printf("[Matmul Blocado] N: %ld | Bloco B: %ld | Tempo: %.4f s | GFLOPS: %.2f\n",
           N, B, tempo, gflops);
    printf("Checksum (amostra C[0][0], C[N-1][N-1]): %.2f %.2f\n",
           C[0], C[(N - 1) * N + (N - 1)]);

    free(A); free(Bm); free(C);
    return 0;
}
