#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Function to perform in-place bit reversal of a number
unsigned int bit_reverse(unsigned int num, int bits) {
    unsigned int reversed_num = 0;
    for (int i = 0; i < bits; i++) {
        reversed_num = (reversed_num << 1) | (num & 1);
        num >>= 1;
    }
    return reversed_num;
}

// Cooley-Tukey FFT algorithm
void fft(complex double *a, int n) {
    // Bit-reversal permutation (parallelized)
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int j = bit_reverse(i, log2(n));
        if (j > i) {
            complex double temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    // Iterative FFT (parallelized)
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
}

int main() {
    int n = 4; // FFT size, must be a power of 2
    complex double a[] = {1, 2, 3, 4}; // Input sequence

    // Perform FFT
    fft(a, n);

    // Display the result
    printf("FFT result:\n");
    for (int i = 0; i < n; i++) {
        printf("(%f, %f)\n", creal(a[i]), cimag(a[i]));
    }

    return 0;
}
