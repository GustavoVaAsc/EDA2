#include <stdio.h>
#include <omp.h>

#define TAM 10000

long work1(int id){

    int i;
    long tmp = 0;

    for(i = 0; i < TAM * TAM; i++){
        tmp *= id;
    }

    return tmp;

}

int work2(int i, int A[]){

    int j;
    int thr = omp_get_num_threads();

    int scalarProduct = 0;
    for (int j = 0; j < thr; j++) {
        scalarProduct += A[j] * A[j];
    }
    return scalarProduct;

}

long work3(int C[], int i){

    int j;
    int thr = omp_get_num_threads();

    int scalarProduct = 0;
    for (int j = 0; j < thr; j++) {
        scalarProduct += C[j] * C[j];
    }
    return scalarProduct;
}

long work4(int id){

    int i;
    long tmp = 0;

    for(i = 0; i < TAM * TAM; i++){
        tmp *= id;
    }

    return tmp;


}

int main(){

    int A[omp_get_num_threads()], B[TAM], C[TAM];
    int id, i;

    #pragma omp parallel shared(A, B, C) private(id)
	{
		id = omp_get_thread_num();
		printf("Thread %d work1()... \n", id);
		A[id] = work1(id);
		#pragma omp barrier
		#pragma omp for
		for (i=0; i<TAM; i++){
			C[i]=work2(i,A);
		}
		printf("Thread %d work2()...\n", id);
		#pragma omp for nowait
		for (i=0; i<TAM; i++){
			B[i]=work3(C,i);
		}
		printf("Thread %d work3()...\n", id);
		printf("Thread %d work4()...\n", id);
		A[id] = work4(id);
	}

    return 0;
}