#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// TAMANHO DO VETOR: 16777216 (2^24)
#define N (1L<<24)


void shell_sort_pass(int *a, long size, int interval) {
    #pragma omp parallel for
    for (int start = 0; start < interval; start++) {
        for (int i = start; i < size; i += interval) {
            int v = a[i];
            int j;
            for (j = i - interval; j >= 0 && a[j] > v; j -= interval) {
                a[j + interval] = a[j];
            }
            a[j + interval] = v;
        }
    }
}

void shell_sort(int *a, long size, int max_threads) {
    int ciura_intervals[] = {701, 301, 132, 57, 23, 10, 4, 1};
    double extend_ciura_multiplier = 2.3;

    omp_set_num_threads(max_threads); // Define o número de threads para OpenMP

    int interval_idx = 0;
    int interval = ciura_intervals[0];
    if (size > interval) {
        while (size > interval) {
            interval_idx--;
            interval = (int)(interval * extend_ciura_multiplier);
        }
    } else {
        while (size < interval) {
            interval_idx++;
            interval = ciura_intervals[interval_idx];
        }
    }

    while (interval > 1) {
        interval_idx++;
        if (interval_idx >= 0) {
            interval = ciura_intervals[interval_idx];
        } else {
            interval = (int)(interval / extend_ciura_multiplier);
        }

        shell_sort_pass(a, size, interval);
    }
}

int main(int argc, char *argv[]) {
    int max_threads = 4; // Define o valor padrão para max_threads
    if (argc > 1) {
        max_threads = atoi(argv[1]);
        if (max_threads < 1) max_threads = 4; // Garante pelo menos uma thread
    }

    srand(123); // Inicializa o gerador de números aleatórios

    int *ints = (int *)malloc(N * sizeof(int));
    if (ints == NULL) {
        fprintf(stderr, "Falha na alocação de memória\n");
        return 1; // Encerra o programa se a alocação falhar
    }

    for (int i = 0; i < N; i++) {
        ints[i] = rand() % 100; // Gera números aleatórios entre 0 e 99
    }

    clock_t start_time, end_time;
    double execution_time;

    start_time = clock(); // Inicia a medição de tempo

    // Aplica Shell Sort
    shell_sort(ints, N, max_threads);

    end_time = clock(); // Finaliza a medição de tempo
    execution_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo decorrido em segundos

    // Verifica se o vetor foi ordenado corretamente
    int i = 1;
    while (i < N) {
        if (ints[i - 1] > ints[i]) {
            printf("\nO vetor NÃO foi ordenado corretamente.\n");
            i = N;
        }
        i++;
    }

    printf("Tempo de execução: %f segundos\n", execution_time);

     free(ints);

    return 0;
}