#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void run_omp_matmul(int n, double* timer, int nthreads) {
    int i, j, k, tid, chunk = 1;
    double * AA = malloc((n * n) * sizeof(double));
    double * BB = malloc((n * n) * sizeof(double));
    double * CC = malloc((n * n) * sizeof(double));

    #define A(i,j) AA[i+j*n]
    #define B(i,j) BB[i+j*n]
    #define C(i,j) CC[i+j*n]

    /*** Spawn a parallel region explicitly scoping all variables ***/
    omp_set_num_threads(nthreads);
    double time = omp_get_wtime();
    
    /* Initialize A, B, and C matrices */
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            A(i,j) = i+j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            B(i,j)= i*j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C(i,j) = 0.;

    /*** Do matrix multiply sharing iterations on outer loop ***/
    #pragma omp parallel for schedule(dynamic) collapse(2)
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            for (k = 0; k < n; ++k)
                C(i,j) += A(i,k)*B(k,j);
        }
    }
    /*** Do matrix multiply sharing iterations on outer loop ***/
    
    time = omp_get_wtime() - time;
    *timer += time;
    free(AA);
    free(BB);
    free(CC);

}

int main(int argc, char *argv[]) {
    long int n = atoi(argv[1]);
    int nthreads = atoi(argv[2]), iterations = atoi(argv[3]);
    double timer = 0.0;

    for (int i=0; i < iterations; i++) {
        run_omp_matmul(n, &timer, nthreads);
    }
    timer = timer / iterations;

    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        int numthrds = omp_get_num_threads();
        if (id == 0) nthreads = numthrds; /* Save number of threads used from only one thread. */
    }

    printf("size n   = %d\naverage  = %.8f secs\nnum thrds = %d",
        n, timer, nthreads);
    printf ("\nDone.\n");
    return 0;
}
