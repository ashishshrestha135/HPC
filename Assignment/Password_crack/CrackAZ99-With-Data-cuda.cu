#include <cuda_runtime_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
/***********************************************************************
**

  Compile with:
    nvcc -o CrackAZ99-With-Data-cuda CrackAZ99-With-Data-cuda.cu 

    ./CrackAZ99-With-Data-cuda

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is 
  identical to the plain text password string stored in the program. 
  Otherwise,it returns 0.
*****************************************************************************/

__device__ int is_a_match(char *attempt) {
  char plain_password[] = "ML42";
  
  char *a = attempt;
  char *p = plain_password;
  
  while(*a == *p) {
    if(*a == '\0') {
      return 1;
    }
    a++;
    p++;
  }
  return 0;
}

/****************************************************************************
  The kernel function run in 675 threads uses 
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

__global__ void kernel() {
  char alpha[26]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
 
  char num[10] = {'0','1','2','3','4','5','6','7','8','9'};
  
  char result[5];
  result[4] = '\0';
  int e, f;
  for(e=0;e<=9;e++) {
     for(f=0; f<9; f++) {
          result[0] = alpha[blockIdx.x];
          result[1] = alpha[threadIdx.x];
          result[2] = num[e];
          result[3] = num[f];
          if(is_a_match(result)) {
          printf("password found: %s\n", result);
          } else {
               //printf("tried: %s\n", result);
          }
     }
  }
}


 int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]) {
  struct timespec start, finish
;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
  
     kernel <<<26, 26>>>();
  cudaThreadSynchronize();
    
    
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
                                         (time_elapsed/1.0e9)); 


  return 0;
}
