#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N (2<<24)

void shell_sort_pass(int *a, long size, int interval) {
    for (int i = interval; i < size; i++) {
        /* Insert a[i] into the sorted sublist */
        int v = a[i];
        int j;

        for (j = i - interval; j >= 0 && a[j] > v; j -= interval) {
            a[j + interval] = a[j];
        }
        a[j + interval] = v;
    }
}

void shell_sort(int *a, long size) {
    int ciura_intervals[] = {701, 301, 132, 57, 23, 10, 4, 1};
    double extend_ciura_multiplier = 2.3;

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

int main() {
    srand(123); // Inicializa o gerador de números aleatórios

    int* ints = (int*)malloc(N * sizeof(int));
    if (ints == NULL) {
        printf("Falha ao alocar memória.\n");
        return -1; // Encerra o programa se a alocação falhar
    }
    for (int i = 0; i < N; i++) {
        ints[i] = rand() % 100; // Gera números aleatórios entre 0 e 99
    }

    clock_t start_time, end_time;
    double execution_time;

    start_time = clock(); // Inicia a medição de tempo

    // Aplica Shell Sort sequencial
    shell_sort(ints, N);

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