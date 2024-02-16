#include<stdio.h>
#include<omp.h>

int main(){
	int id;
	#pragma omp parallel
	{
		#pragma omp master
		{
			id = omp_get_thread_num();
			printf("Master thread %d\n", id);
		}

		id = omp_get_thread_num();
		printf("Parallel thread %d\n", id);
		
	}
}