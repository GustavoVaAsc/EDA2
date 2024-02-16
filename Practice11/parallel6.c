#include <stdio.h>
#include <omp.h>
#include <limits.h>
#include <stdlib.h>

#define NUM_THREADS 16
#define N 6

int calculateTotalDistance(int graph[N][N], int route[N]) {
    int totalDistance = 0;
    for (int i = 0; i < N - 1; i++) {
        if (graph[route[i]][route[i + 1]] == 0) {
            return INT_MAX;
        }
        totalDistance += graph[route[i]][route[i + 1]];
    }
    totalDistance += graph[route[N - 1]][route[0]];
    return totalDistance;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void tspBruteForce(int graph[N][N], int route[N], int currentCity, int *minDistance) {
    if (currentCity == N - 1) {
        int distance = calculateTotalDistance(graph, route);
        if (distance < *minDistance) {
            *minDistance = distance;
        }
        return;
    }

    for (int i = currentCity; i < N; i++) {
        swap(&route[currentCity], &route[i]);
        tspBruteForce(graph, route, currentCity + 1, minDistance);
        swap(&route[currentCity], &route[i]);
    }
}

int big_job(int id) {

    int graph[N][N] = {
        {0, 10, 15, 20, 25, 30},
        {10, 0, 35, 40, 45, 50},
        {15, 35, 0, 55, 60, 65},
        {20, 40, 55, 0, 70, 75},
        {25, 45, 60, 70, 0, 85},
        {30, 50, 65, 75, 85, 0}
    };
    /*
    int graph[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int distance = rand() % 91 + 10;
            graph[i][j] = distance;
            graph[j][i] = distance;
        }
        graph[i][i] = 0;
    }
    */
    int route[N];
    for (int i = 0; i < N; i++) {
        route[i] = i;
    }

    int currentCity = 0;
    int minDistance = INT_MAX;

    tspBruteForce(graph, route, currentCity, &minDistance);

    return minDistance;
}

double consume(int id){
    double res = (double) big_job(id);
    return res;
}


int main(){
    omp_set_num_threads(NUM_THREADS);

    int iterations = 100;
    double res = 0.0;
    double rtime = omp_get_wtime();

    #pragma omp parallel
    {
        double B;
        int i, id, threadssss;

        id = omp_get_thread_num();
        threadssss = NUM_THREADS;

        for (i=id; i<iterations; i+=threadssss){
            B = (double) big_job(id);
            printf("Thread %d, time %f, res = %f\n", id, omp_get_wtime()-rtime, res);
            //#pragma omp critical
            res += consume(id);
            printf("Thread %d after consume, time %f, res = %f\n", id, omp_get_wtime()-rtime, res);
        }
    }

    printf("res = %f\n", res);

    return 0;
}
