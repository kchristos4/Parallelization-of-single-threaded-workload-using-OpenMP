//KARAGIANNIDIS CHRISTOS 4375
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#define UPTO 10000000

long int count,      /* number of primes */
         lastprime;  /* the last prime found */


void serial_primes(long int n) {
	long int i, num, divisor, quotient, remainder;

	if (n < 2) return;
	count = 1;                         /* 2 is the first prime */
	lastprime = 2;

	for (i = 0; i < (n-1)/2; ++i) {    /* For every odd number */
		num = 2*i + 3;

		divisor = 1;
		do 
		{
			divisor += 2;                  /* Divide by the next odd */
			quotient  = num / divisor;  
			remainder = num % divisor;  
		} while (remainder && divisor <= quotient);  /* Don't go past sqrt */

		if (remainder || divisor == num) /* num is prime */
		{
			count++;
			lastprime = num;
		}
	}

}


void openmp_primes(long int n) {
	long int i, num, divisor, quotient, remainder;
	
	if (n < 2) return;
	
	count = 1;                         /* 2 is the first prime */
	lastprime = 2;

	#pragma omp parallel private(num,quotient,divisor,remainder) reduction(max:lastprime) num_threads(4)
	{
	#pragma omp for schedule(dynamic,5)
	for (i = 0; i < (n-1)/2; ++i) {    /* For every odd number */
		num = 2*i + 3;

		divisor = 1;
		do 
		{
			divisor += 2;                  /* Divide by the next odd */
			quotient  = num / divisor;  
			remainder = num % divisor;  
		} while (remainder && divisor <= quotient);  /* Don't go past sqrt */

			
		
		if (remainder || divisor == num) /* num is prime */
		{
			#pragma omp critical
			{
			count++;
			lastprime = num;
			}
			//printf("%d\t %ld\t%ld\n", omp_get_thread_num (),count,lastprime);
			
		}
	}
	
	}
	/* 
	 * Parallelize the serial algorithm but you are NOT allowed to change it!
	 * Don't add/remove/change global variables
	 */
}


int main()
{
	struct timeval start, end;
	double diff;
	printf("Serial and parallel prime number calculations:\n\n");
	/* Time the following to compare performance 
	 */
	
    gettimeofday(&start, NULL);
	serial_primes(UPTO); 
	gettimeofday(&end, NULL);
	diff = (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);
	printf("[serial] count = %ld, last = %ld (time = %f)\n", count, lastprime, diff);
	


	gettimeofday(&start, NULL);
	openmp_primes(UPTO);
	gettimeofday(&end, NULL);
	diff = (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);
	printf("[openmp] count = %ld, last = %ld (time = %f)\n", count, lastprime, diff);
	
	return 0;
}