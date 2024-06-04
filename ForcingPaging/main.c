// A program to measure the amount of time taken caused by page swapping.
// CS574
// Author: Long Tran
// Date: Feb 10, 2024
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 
#include <math.h>
#include <string.h>

enum unit {
    GB,
    B
};

long max_size = 0;
enum unit max_unit = 0;
long chunk_size = 0;
enum unit chunk_unit = 0;

long max_size_in_bytes = 0;
long chunk_increase_in_bytes = 0;

long long get_duration(struct timespec start, struct timespec end) {
   long j = end.tv_sec - start.tv_sec;
   long k = end.tv_nsec - start.tv_nsec;
   return j * 1000000000 + k;
} // end get_duration

char ** chunks = NULL;

void * touch_memory(void *arg) {
    srand(time(NULL));
    int index = *((int *) arg);
    free(arg);

    for (int i = 0; i < index; ++i) {
        memset(chunks[i], 'a', chunk_increase_in_bytes);
    } // end for i 
    return NULL;
} // end touch_memory

void start_experiment() {
    int num_chunks = max_size_in_bytes / chunk_increase_in_bytes;
    pthread_t thread_id;
    chunks = (char **) malloc(num_chunks * sizeof(char *));

    for (int i = 0; i < num_chunks; ++i) {
        for (int j = 0; j < i; ++j) {
            for (int k = 0; k < chunk_increase_in_bytes; ++k) {
                chunks[j][k] = ('a' + j) % 26;
            } // end for k
        } // end for j 

        int * value = malloc(sizeof(int));
        *value = i;
        pthread_create(&thread_id, NULL, touch_memory, (void*) value); 

        // timer start here
        struct timespec start, end; 
        clock_gettime(CLOCK_MONOTONIC, &start);
        chunks[i] = (char *) malloc(chunk_increase_in_bytes);
        for (int j = 0; j < chunk_increase_in_bytes; ++j) {
            chunks[i][j] = 'a';
        } // end for j
        // timer end here
        clock_gettime(CLOCK_MONOTONIC, &end);
        long long duration = get_duration(start, end);
        printf("%d, %lld\n", i, duration);

        pthread_join(thread_id, NULL);
    } // end for i

    for (int i = 0; i < num_chunks; ++i) {
        free(chunks[i]);
    } // end for i
    free(chunks);
}

int main(int argc, char *argv[]) 
{ 
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <GB Max Units to Allocate> > <Chunk Size Increase> <Unit>\n", argv[0]);
        fprintf(stderr, "Usage: %s <Unit>: gb or b\n", argv[0]);
        return 1;
    } // end if
    max_size = atoi(argv[1]);
    chunk_size = atoi(argv[2]);

    if (strcmp(argv[3], "gb") == 0) {
        chunk_unit = GB;
    } else if (strcmp(argv[3], "b") == 0) {
        chunk_unit = B;
    } else {
        fprintf(stderr, "Usage: %s <Unit>: gb or b\n", argv[0]);
        return 1;
    } // end if

    if (chunk_unit == GB) {
        chunk_increase_in_bytes = chunk_size * 1024 * 1024 * 1024;
    } else {
        chunk_increase_in_bytes = chunk_size;
    } // end if

    max_size_in_bytes = max_size * 1024 * 1024 * 1024;  

    start_experiment();

    // exit
    return 0;
} // end main
