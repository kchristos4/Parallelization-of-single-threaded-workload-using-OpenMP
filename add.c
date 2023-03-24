#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#define N 1000000

int counter_serial = 0;
int counter_parallel = 0;

void add_serial(){

	for(int i=0;i<N;i++){
		counter_serial++;
	}
}
void add_parallel(){
	#pragma omp parallel num_threads(4)
	{
		#pragma omp taskloop
		for (int i=0;i<N;i++){
			#pragma omp atomic
				counter_parallel++;
			}
		
	}

}



int main(int argc, char const *argv[])
{
	struct timeval start, end;
	double diff;
	printf("Serial and parallel number calculations:\n\n");
	/* Time the following to compare performance 
	 */
	
    gettimeofday(&start, NULL);
	add_serial(); 
	gettimeofday(&end, NULL);
	diff = (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);
	printf("[serial] counter serial  = %d time = %f\n",counter_serial,  diff);
	


	gettimeofday(&start, NULL);
	add_parallel();
	gettimeofday(&end, NULL);
	diff = (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);
	printf("[openmp] counter parallel = %d time = %f\n",counter_parallel,  diff);
	
	return 0;
}