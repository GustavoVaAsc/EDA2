#include <stdio.h>
#include <omp.h>



int section_a(int n){
    printf("Hi, im in section A!! :D, i love this number btw: %d\n", n);
}

int section_b(int n){
    printf("Hi, im in section B!! :D, i love this number btw: %d\n", n);
}

int section_c(int n){
    printf("Hi, im in section C!! :D, i love this number btw: %d\n", n+n);
}


int main(){

    int id;

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            section_a(omp_get_thread_num());
            #pragma omp section
            section_b(omp_get_thread_num());
            #pragma omp section
            section_c(omp_get_thread_num());
        }

        id = omp_get_thread_num();
        printf("Parallel block thread %d.\n", id);
    }

    return 0;
    
}