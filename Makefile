CC = gcc
CFLAGS_O0 = -O0 -Wall
CFLAGS_O3 = -O3 -Wall
PTHREAD = -pthread

TARGETS = varredura_linha varredura_coluna \
          matmul_padrao_O0 matmul_padrao_O3 \
          matmul_bloco_O0 matmul_bloco_O3 \
          matmul_pthreads matmul_pthreads_bloco

all: $(TARGETS)

varredura_linha: varredura_linha.c
	$(CC) $(CFLAGS_O3) $< -o $@

varredura_coluna: varredura_coluna.c
	$(CC) $(CFLAGS_O3) $< -o $@

matmul_padrao_O0: matmul_padrao.c
	$(CC) $(CFLAGS_O0) $< -o $@

matmul_padrao_O3: matmul_padrao.c
	$(CC) $(CFLAGS_O3) $< -o $@

matmul_bloco_O0: matmul_bloco.c
	$(CC) $(CFLAGS_O0) $< -o $@

matmul_bloco_O3: matmul_bloco.c
	$(CC) $(CFLAGS_O3) $< -o $@

matmul_pthreads: matmul_pthreads.c
	$(CC) $(CFLAGS_O3) $(PTHREAD) $< -o $@

matmul_pthreads_bloco: matmul_pthreads_bloco.c
	$(CC) $(CFLAGS_O3) $(PTHREAD) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
