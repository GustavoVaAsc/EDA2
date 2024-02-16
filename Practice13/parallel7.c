#include <stdio.h>
#include <omp.h>

#define RA 5
#define CA 5
#define RB 5
#define CB 5

// Parallel paradigm: Loop paralelism

void multiply(int a[RA][CA], int b[RB][CB], int c[RA][CB]) {
    int i, j, k;
    #pragma omp parallel for private(i, j, k) shared(a, b, c)
    for (i = 0; i < RA; i++) {
        for (j = 0; j < CB; j++) {
            c[i][j] = 0;
            for (k = 0; k < CA; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    int a[RA][CA], b[RB][CB], c[RA][CB];
    int i, j;
    printf("Generate matrix A:\n");
    for (i = 0; i < RA; i++) {
        for (j = 0; j < CA; j++) {
            a[i][j] = (i + j)*2;
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("Generate matrix B:\n");
    for (i = 0; i < RB; i++) {
        for (j = 0; j < CB; j++) {
            b[i][j] = i - j;
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
    multiply(a, b, c);
    printf("Result matrix:\n");
    for (i = 0; i < RA; i++) {
        for (j = 0; j < CB; j++) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }
    return 0;
}