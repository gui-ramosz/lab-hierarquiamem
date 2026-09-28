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
    if (!A) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        return 1;
    }

    /* Inicializacao deterministica padronizada (identica a varredura_linha.c) */
    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            A[i * N + j] = (double) (i + j);
        }
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    long pares = 0;
    for (long j = 0; j < N; j++) {
        for (long i = 0; i < N; i++) {
            if ((long) A[i * N + j] % 2 == 0) {
                pares++;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[VarreduraColuna] N=%ld | Pares=%ld | Tempo: %.4f s\n", N, pares, tempo);

    free(A);
    return 0;
}
