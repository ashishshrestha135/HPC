#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <crypt.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o CrackAZ99-With-3initials CrackAZ99-With-3initials.c -lcrypt

  If you want to analyse the results then use the redirection operator
 to send
  output to a file that you can view using an editor or the less
 utility:

    ./CrackAZ99-With-3initials > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$.OnsM0.pEomFoalYh1Tz0da3BPcVeBs6fn3fn6Aq8XMjLMc5aImRqL9hBnjFpCVLigSq9dYwAelFBufipopxY1",

"$6$KB$a3I0PZzxK72KmUEF9BwCDgavNTcKF7qMyisIX92DAGRCeiU4eAngEL7TSC/v.vAix7d0j1jFV3stlwZUohpEo0",

"$6$KB$hn2ozvmpb8pvNu.G7pewcZb6xu.iQBuE9MywKWKPgsALV.gpJnzo8NQqJBs7yvs13jPV2dficoIfumz04V5uH/",

"$6$KB$o1ZCfk2QE6ENjSr9AViCaiPy6Ln1.pkNEorytYnS5gEAAk4dYSTUomM5YGbXSs6Zg1ObJ3HiraiyI0ONH1oAb0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted){
  int w, x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

for(w='A'; w<='Z'; w++){
  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
       for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%c%02d", w, x, y, z);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
       }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

// Calculate the difference between two times. Returns zero on
// success and the time difference through an argument. It will 
// be unsuccessful if the start time is after the end time.

int time_difference(struct timespec *start, 
                    struct timespec *finish, 
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

int main(int argc, char *argv[]){
  int i;
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  
    for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
    }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs or %0.9lfmin\n", time_elapsed,
                                         (time_elapsed/1.0e9),((time_elapsed/1.0e9)/60)); 


  


  return 0;
}


