// Program to spawn from 1 to MAX_THREADS_RUN number of threads
// Then fill an array 1 << 31 times. 
// Each thread number is run five times.
// The run time of each run is printed to STDERR
// The program will also calculate the average and standard deviation. 
// Then, it will be printed out on STDOUT. 
// Author: Long Tran
// Date: Feb 10, 2024
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 
#include <math.h>

#define MAX_THREADS_RUN 32 
#define AVERAGE_RUN 5

double get_average(long long* values, int length) {
    double result = 0;
    for (int i = 0; i < length; ++i) {
        result += (values[i] / length);
    } // end for i

    return result;
} // end average
  
double std(long long* values, int length, double mean) {
    double result = 0;
    for (int i = 0; i < length; ++i) {
        double diff = values[i] - mean;
        diff = diff * diff;
        diff = diff / (length-1);
        result += diff;
    } // end for i

    return sqrt(result);
} // end average

void *myThreadFunction(void *vargp) 
{ 
    unsigned int length = 1 << 20; 
    int array[10];
    for (int i = 0; i < length; ++i) {
        array[i % 10] = 0;
    } // end for i

    for (int i = 0; i < 10; ++i) {
        array[0] += array[i];
    } // end for i
    return NULL; 
} // end myThreadFunction

void runThreads(int threadNum) {
    // thread id
    pthread_t * threads = malloc(threadNum * sizeof(pthread_t)); 

    // create threads
    for (int i = 0; i < threadNum; ++i) {
        // create thread
        pthread_create(&threads[i], NULL, myThreadFunction, NULL); 
    } // end for i

    // wait for threads
    for (int i = 0; i < threadNum; ++i) {
        // get thread id
        pthread_t thread_id = threads[i];
        // wait for a thread
        pthread_join(thread_id, NULL); 
    } // end for i

    // free the memory
    free(threads);
} // end runThreads

long long get_duration(struct timespec start, struct timespec end) {
   long j = end.tv_sec - start.tv_sec;
   long k = end.tv_nsec - start.tv_nsec;
   return j * 1000000000 + k;
} // end get_duration
   
int main() 
{ 
    for (int threadNum = 1; threadNum <= MAX_THREADS_RUN; threadNum++) {
        long long durations[AVERAGE_RUN];
        for (int round = 0; round < AVERAGE_RUN; ++round) {
            struct timespec start, end;

            // run the process
            clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
            runThreads(threadNum);
            clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */

            // get duration
            long long duration = get_duration(start, end);

            durations[round] = duration;
            
            fprintf(stderr, "Run %d with %d: %lld nanosec\n", round, threadNum, duration);
        } // end for round 
        double average = get_average(durations, AVERAGE_RUN); 
        double standardDeviation = std(durations, AVERAGE_RUN, average); 
        printf("%d, %f, %f\n", threadNum, average, standardDeviation);
    } // end for threadNum 

    // exit
    return 0;
} // end main
