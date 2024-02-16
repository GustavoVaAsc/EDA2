#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

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
    int flag = 0,tmp_flag;
    long comparisons = 0;
    double startT;
    startT = omp_get_wtime();

    #pragma omp parallel sections num_threads(2)
	{
		#pragma omp section
		{
			fillRandomArray();
			#pragma omp flush
			#pragma omp atomic write
			flag = 1;
			#pragma omp flush(flag)
		}
		#pragma omp section 
		{
			while (1) {
                printf("UPDATING TMP FLAG...\n");
				#pragma omp flush(flag)
				#pragma omp atomic read
				tmp_flag = flag;
                printf("W : FLAG - %d\n",tmp_flag);
				if (tmp_flag == 1)
					break;
			}
			#pragma omp flush
			comparisons = bubbleSort();
		}
	}
    startT = omp_get_wtime() - startT;
    printf("SUMA: %d - TIEMPO: %f", comparisons, startT);
    return 0;
}