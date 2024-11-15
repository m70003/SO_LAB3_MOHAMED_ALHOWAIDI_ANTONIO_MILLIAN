/* producer_consumer.c */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TOTAL_NUMBERS  39    // Total numbers to be produced and consumed

int numbers[TOTAL_NUMBERS]; // Vector to store the numbers

void *produce_numbers(void *arg)
{
  int  i;
  
  for (i=0; i<TOTAL_NUMBERS; i++){        
    numbers[i] = random() % 100;         // Produce an integer between 0 and 99
    printf("Thread_produce_numbers: Produces in position %d number %d \n",i,numbers[i]);
  }
  pthread_exit(0);
}

void *count_numbers(void *arg)
{
  int i, total_even_numbers=0, total_odd_numbers=0;
  
  for (i=0; i<TOTAL_NUMBERS; i++){        
    if ((numbers[i]%2) == 0) 
    {
       total_even_numbers++;
       printf("          Thread_count_numbers: Read in position %d number %d \t(even)   \n", i, numbers[i]);
    } else {
       total_odd_numbers++;
       printf("          Thread_count_numbers: Read in position %d number %d \t(odd)   \n", i, numbers[i]);
    }
  
  }
  printf("Thread_count_lines: Total even numbers= %d Total odd numbers = %d Total numbers = %d \n", total_even_numbers, total_odd_numbers, total_even_numbers+total_odd_numbers);
  pthread_exit(0);
}

int main()
{
    pthread_t thread_produce_numbers; // Producer thread 
    pthread_t thread_count_numbers;   // Consumer thread


    // The thread_count_numbers is created when the thread_produce_numbers ends, so that syncronization is not required
    if  ( pthread_create(&thread_produce_numbers, NULL, produce_numbers, NULL) ) {
      perror("Error creating producer thread.");
      exit(-1);
    }
    if  ( pthread_join(thread_produce_numbers,NULL)) {
      perror("Error joining producer thread.");
      exit(-1);
    }

    if  ( pthread_create(&thread_count_numbers, NULL, count_numbers, NULL) ) {
      perror("Error creating producer thread.");
      exit(-1);
    }
    if  ( pthread_join(thread_count_numbers,NULL) ) {
      perror("Error joining producer thread.");
      exit(-1);
    }

    exit (0);
}


