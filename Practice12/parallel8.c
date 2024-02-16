#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM 50000
long arr[TAM];

void fillRandomArray() {
    for (int i = 0; i < TAM; i++) {
        arr[i] = rand() % 100;
    }
}

long bubbleSort(){
    int i, j, temp, comparisons = 0;
    for (i = 0; i < TAM-1; i++) {
        for (j = 0; j < TAM-i-1; j++) {
            comparisons++;
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    return comparisons;
}




int main(){
    int i;
    int flag = 0;
    long comparisons = 0;
    double startT;
    startT = omp_get_wtime();

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            fillRandomArray();
            #pragma omp flush
            flag = 1;
            #pragma omp flush(flag)
        }
        #pragma omp section
        {
            #pragma omp flush(flag)
            while (flag == 0) {
                #pragma omp flush(flag)
                printf("W : FLAG - %d\n", flag);
            }
            printf("W : FLAG - %d\n", flag);
            #pragma omp flush
            comparisons = bubbleSort();
        }
    }
    startT = omp_get_wtime() - startT;
    printf("BUBBLESORT COMPARISONS: %d - TIME: %f",comparisons, startT);
    return 0;
}