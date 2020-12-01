#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define N 1024

void matrix(double *a, double *b, double *c, long n)
{
   long i, j, k;
   double r;

   for(k = 0; k < n; k++)
        for(j = 0; j < n; j++){
            r = b[k*n+j];
            for(i= 0; i < n; i++)
                c[i*n+j] += a[i*n+k]*r;
        }      
}



int main(void)
{
  double *ptr_a;
  double *ptr_b;
  double *ptr_c;
  struct timeval tv1, tv2;
  long i,j;

  ptr_a = malloc(N*N*8);
  ptr_b = malloc(N*N*8);
  ptr_c = malloc(N*N*8);

  for(i = 0; i< N; i++)
    for(j =0; j < N; j++)
    {
      ptr_a[i*N+j] = 2.0;
      ptr_b[i*N+j] = -2.0;
      ptr_c[i*N+j] = 0.0;
    }
  gettimeofday(&tv1, NULL);
  matrix(ptr_a, ptr_b, ptr_c, N);
  gettimeofday(&tv2, NULL);
  printf("time elapse %ld s: %ld us\n", (unsigned)tv2.tv_sec - tv1.tv_sec, (unsigned)tv2.tv_usec - tv1.tv_usec);
  return 0;
}