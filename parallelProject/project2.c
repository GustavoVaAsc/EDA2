#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define N 65536 //131072 //65536
complex double serialfftResults[N];
complex double parallelfftResults[N];
complex double taskfftResults[N];

// Function to perform in-place bit reversal of a number
unsigned int bit_reverse(unsigned int num, int bits) {
    unsigned int reversed_num = 0;
    for (int i = 0; i < bits; i++) {
        reversed_num = (reversed_num << 1) | (num & 1);
        num >>= 1;
    }
    return reversed_num;
}

void serialfft(complex double *a, int n, FILE *S, int inst) {
    double start_time = omp_get_wtime();
    // Bit-reversal permutation
    for (int i = 0; i < n; i++) {
        int j = bit_reverse(i, log2(n));
        if (j > i) {
            complex double temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    // Iterative FFT
    for (int m = 2; m <= n; m *= 2) {
        complex double wm = cexp(2.0 * I * M_PI / m);
        for (int k = 0; k < n; k += m) {
            complex double w = 1.0;
            for (int j = 0; j < m / 2; j++) {
                complex double t = w * a[k + j + m / 2];
                complex double u = a[k + j];
                a[k + j] = u + t;
                a[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
    double end_time = omp_get_wtime() - start_time;
    fprintf(S, "%d, %f\n", inst, end_time);

}

void parallelfft(complex double *a, int n, FILE *P,int inst) {
    double start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int j = bit_reverse(i, log2(n));
        if (j > i) {
            complex double temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    for (int m = 2; m <= n; m *= 2) {
        complex double wm = cexp(2.0 * I * M_PI / m);
        #pragma omp parallel for
        for (int k = 0; k < n; k += m) {
            complex double w = 1.0;
            for (int j = 0; j < m / 2; j++) {
                complex double t = w * a[k + j + m / 2];
                complex double u = a[k + j];
                a[k + j] = u + t;
                a[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
    double end_time = omp_get_wtime() - start_time;
    fprintf(P, "%d, %f\n", inst, end_time);

}

void taskfft(complex double *a, int n, FILE *T, int inst) {
    double start_time = omp_get_wtime();

    #pragma omp parallel
    #pragma omp single
    #pragma omp taskloop
    for (int i = 0; i < n; i++) {
        int j = bit_reverse(i, log2(n));
        if (j > i) {
            complex double temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    for (int m = 2; m <= n; m *= 2) {
        complex double wm = cexp(2.0 * I * M_PI / m);
        #pragma omp parallel
        #pragma omp single
        #pragma omp taskloop
        for (int k = 0; k < n; k += m) {
            complex double w = 1.0;
            for (int j = 0; j < m / 2; j++) {
                complex double t = w * a[k + j + m / 2];
                complex double u = a[k + j];
                a[k + j] = u + t;
                a[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
    double end_time = omp_get_wtime() - start_time;
    fprintf(T, "%d, %f\n", inst, end_time);
}

int main(){
    FILE *S = fopen("serialTime.csv","w");
    if(S == NULL){
        printf("Error opening serial file\n");
        return 1;
    }
    fprintf(S, "Polynomial degree, Time\n");
    int n = 65536; //131072;
    int nxtpow = 2;
    for(int i=1; i<=n; i++){
        if(i > nxtpow){
            nxtpow<<=1;
            printf("Input size (serial): %d \n",nxtpow);
        }
        complex double signal[nxtpow];

        for (int j=0; j<nxtpow; j++){
            if(j<i){
                signal[j] = (j+1)%10;
            }else{
                signal[j] = 0;
            }
        }
        serialfft(signal, nxtpow,S,i);

        if (i == n){
            printf("Final loop :D\n");
            for (int j=0; j<i; j++){
                serialfftResults[j] = signal[j];
            }
        }
    }

    fclose(S);

    FILE *P = fopen("parallelTime.csv","w");
    if(S == NULL){
        printf("Error opening parallel file\n");
        return 1;
    }
    fprintf(P, "Polynomial degree, Time\n");
    n = 65536; //131072;
    nxtpow = 2;
    for(int i=1; i<=n; i++){
        if(i > nxtpow){
            nxtpow<<=1;
            printf("Input size (parallel): %d \n",nxtpow);
        }
        complex double signal[nxtpow];

        for (int j=0; j<nxtpow; j++){
            if(j<i){
                signal[j] = (j+1)%10;
            }else{
                signal[j] = 0;
            }
        }
        parallelfft(signal, nxtpow,S,i);

        if (i == n){
            printf("Final loop :D\n");
            for (int j=0; j<i; j++){
                parallelfftResults[j] = signal[j];
            }
        }
    }

    fclose(P);

    FILE *T = fopen("taskTime.csv","w");
    if(S == NULL){
        printf("Error opening task file\n");
        return 1;
    }
    fprintf(T, "Polynomial degree, Time\n");
    n = 65536; //131072;
    nxtpow = 2;
    for(int i=1; i<=n; i++){
        if(i > nxtpow){
            nxtpow<<=1;
            printf("Input size (task): %d \n",nxtpow);
        }
        complex double signal[nxtpow];

        for (int j=0; j<nxtpow; j++){
            if(j<i){
                signal[j] = (j+1)%10;
            }else{
                signal[j] = 0;
            }
        }
        
        taskfft(signal, nxtpow,T,i);

        if (i == n){
            printf("Final loop :D\n");
            for (int j=0; j<i; j++){
                taskfftResults[j] = signal[j];
            }
        }
    }

    fclose(P);
    
    FILE *RS = fopen("resultsSerial.txt", "w");
    FILE *RP = fopen("resultsParallel.txt", "w");
    FILE *RT = fopen("resultsTask.txt", "w");
    fprintf(RS, "Real,Imaginary\n");
    fprintf(RP, "Real,Imaginary\n");
    fprintf(RT, "Real,Imaginary\n");
    for(int i=0; i<N; i++){
        fprintf(RS, "%.5f,%.5f\n",creal(serialfftResults[i]), cimag(serialfftResults[i]));
        fprintf(RP, "%.5f,%.5f\n",creal(parallelfftResults[i]), cimag(parallelfftResults[i]));
        fprintf(RT, "%.5f,%.5f\n",creal(taskfftResults[i]), cimag(taskfftResults[i]));
    }

    fclose(RS);
    fclose(RP);
    fclose(RT);
    return 0;
}

