#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Function to perform in-place bit reversal of a number
unsigned int bit_reverse(unsigned int num, int bits) {
    unsigned int reversed_num = 0; // 3 || 1
    for (int i = 0; i < bits; i++) { // 5(Bits + 1) || 1
        reversed_num = (reversed_num << 1) | (num & 1); // 5(Bits) || 1
        num >>= 1; // 3 || 1
    }
    return reversed_num; // 1 || 1
}

// bit reverse TIME complexity: 10 (Bits) + 12 = O(n)
// bit reverse SPACE complexity: 5  = O(1)

// Cooley-Tukey FFT algorithm
void fft(complex double *a, int n) {
    // Bit-reversal permutation
    for (int i = 0; i < n; i++) { // 5 (n+1) || 1
        int j = bit_reverse(i, log2(n)); // 5(n * O(Bits) + 1) || 1 // As the input is limited by log2n O(Bits) = log n
        if(j > i){ // 4(n) || 1
            complex double temp = a[i]; // 6(n) || 1
            a[i] = a[j]; // 4(n) || 1
            a[j] = temp; // 4(n) || 1
        }
    }

    // Iterative FFT
    for (int m = 2; m <= n; m *= 2) { //Constantly multiplying variable m by 2  4(log(n) - 1) || 1
        complex double wm = cexp(2.0 * I * M_PI / m); // 6(log n) || 1
        for (int k = 0; k < n; k += m) { // As m increases logarithmicly 4(n * log(n) + 1) || 1
            complex double w = 1.0; // 4(n * log n) || 1
            for (int j = 0; j < m / 2; j++) { // m = log(n) so... 4(log(n) * (n*log(n))+1) || 1
                // log^2(n) = 2 log(n)
                complex double t = w * a[k + j + m / 2]; // 18 (n * log(n)) || 1
                complex double u = a[k + j];// 12 (n * log(n)) || 1
                a[k + j] = u + t; // 12 (n * log(n)) || 1
                a[k + j + m / 2] = u - t; // 12 (n * log(n)) || 1
                w *= wm; // 6(n * log(n)) || 1
            }
        }
    }
}

// FFT TIME complexity: 81 (n*log(n)) + 17(n) + 6 log(n) + 5 = O(n*log(n))
// FFT SPACE complexity: 16 = O(1)

int main() {
    int n = 8; // FFT size, must be a power of 2
    complex double a[] = {1,2,3,4}; // Input sequence

    // Perform FFT
    fft(a, n);

    // Display the result
    printf("FFT result:\n");
    for (int i = 0; i < n; i++) {
        printf("(%f, %f)\n", creal(a[i]), cimag(a[i]));
    }

    return 0;
}
