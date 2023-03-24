//KARAGIANNIDIS CHRISTOS 4375
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#define N 1024
#define M 1024
#define K 1024

int A[N][M], B[M][K], C[N][K];

void matmul_serial();
void matmul_OMP_taskloop();

static
void init_arrays()
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            A[i][j] = 2;
   
    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
            B[i][j] = 2;
}

static
void zero_arrays()
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            C[i][j] = 0;;
   
    
}


static
void print_results()
{
    int i, j;
    int first,final;
    first = C[0][0];
    final = first;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < K; j++){
            if(C[i][j]!=first)
                final=C[i][j];
        }
        
    }
    printf("first = %d\t final = %d\n",first,final );
    if(first==final)
        printf("All multiplications in matrix C were correct\n");
    else
        printf("There is a wrong result in matrix C");
}

/* Serial matrix multiplication; do not modify. */
void matmul_serial()
{
    int i, j, k;

    for (i = 0; i < N; ++i) 
        for (j = 0; j < K; ++j) 
            for (k = 0; k < M; ++k) 
                C[i][j] += A[i][k] * B[k][j];
}

/* OpenMP matrix multiplication */
void matmul_OMP_taskloop()
{
    int i, j, k;

    /* 1. num_threads not specified, thus it will be 
     *    set to the compiler default, or to OMP_NUM_THREADS env var.
     * 2. shared(A,B,C) & private(i) are already the default behavior
     *    thus can be omitted.
    */
    #pragma omp parallel num_threads(4)
    {
        #pragma omp taskloop private(i,j,k) shared(A,B,C)
            for (i = 0; i < N; i++){
                for (j = 0; j < K; j++){        
                    for (k = 0; k < M; ++k) 
                        #pragma omp atomic
                        C[i][j] += A[i][k] * B[k][j];
            
                }
            }
    }
}

static
double timeit(void (*func)())
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    func();
    gettimeofday(&end, NULL);
    return (double) (end.tv_usec - start.tv_usec) / 1000000 
        + (double) (end.tv_sec - start.tv_sec);
}

int main(int argc, char *argv[])
{
    int i, j;
    double exectime, exectimepar;

    if (N < 1 || M < 1 || K < 1)
    {
        fprintf(stderr, "Wrong dimensions; exiting.\n");
        exit(1);
    }

    printf("matmul (A[%d][%d], B[%d][%d], C[%d][%d])\n\n",
        N, M, M, K, N, K);

    init_arrays();

    /* Matmul-serial timing */
    exectime = timeit(matmul_serial);
    print_results();
    printf("Execution time (serial): %lf\n", exectime);
    printf("___________________________________________________________________________\n\n");
    zero_arrays();
    /* Matmul-OMP timing */
    exectimepar = timeit(matmul_OMP_taskloop);

    print_results();
    printf("Execution time (parallel): %lf\n", exectimepar);

    
}