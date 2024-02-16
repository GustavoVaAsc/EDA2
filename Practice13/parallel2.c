#include<stdio.h>
#include<omp.h>

// Parallel paradigm: Loop parallelism

void func(){
    int tmp = 0;

    printf("Tmp before loop: %d\n\n",tmp);
    #pragma omp parallel for private(tmp)
        for (int j = 0; j<1000; ++j){
            tmp += j;
            printf("Thread %d: tmp = %d\n", omp_get_thread_num(), tmp);
        }
    printf("Tmp after loop: %d\n", tmp);
}

int main(){
    func();
}