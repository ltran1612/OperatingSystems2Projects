// Program to spawn from 1 to MAX_THREADS_RUN number of threads
// Then fill an array 1 << 20 times. 
// Each thread number is run five times.
// The run time of each run is printed to STDERR
// The program will also calculate the average and standard deviation. 
// Then, it will be printed out on STDOUT. 
// Author: Long Tran
#include <stdio.h> 
#include <stdlib.h>  
#include <windows.h> 
#include <math.h>
#include <heapapi.h>
#include <process.h>

#define MAX_THREADS_RUN 64
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

DWORD WINAPI myThreadFunction(void *vargp) 
{ 
    int length = 1 << 20; 
    int array[10];
    for (int i = 0; i < length; ++i) {
        array[i % 10] = 0;
    } // end for i

    for (int i = 0; i < 10; ++i) {
        array[0] += array[i];
    } // end for i

    return 0;
} // end myThreadFunction

//https://learn.microsoft.com/en-us/copp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170&viewFallbackFrom=vs-2019
//https://learn.microsoft.com/en-us/windows/win32/procthread/creating-threads
void runThreads(int threadNum) {
    // thread id
    HANDLE * threads = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(HANDLE) * threadNum);

    // create threads
    for (int i = 0; i < threadNum; ++i) {
        // create thread
        threads[i] = CreateThread(NULL, 0, myThreadFunction, NULL, 0, NULL);
    } // end for i

    // wait for threads
    WaitForMultipleObjects(threadNum, threads, TRUE, INFINITE);

    // free mem and close handle
    for (int i = 0; i < threadNum; ++i) {
        CloseHandle(threads[i]);
    } // end for i
    // free mem
    HeapFree(GetProcessHeap(), 0, threads);
    threads = NULL;
} // end runThreads
   
int main() 
{ 
    for (int threadNum = 1; threadNum <= MAX_THREADS_RUN; threadNum++) {
        long long durations[AVERAGE_RUN];
        for (int round = 0; round < AVERAGE_RUN; ++round) {
            LARGE_INTEGER liFrequency = {0};
            // Get the Frequency
            if(QueryPerformanceFrequency(&liFrequency))
            {
                // Start Timing
                // adapted from https://stackoverflow.com/questions/12956608/get-time-in-nanoseconds-with-c-on-windows-without-chrono
                LARGE_INTEGER liStart = {0};
                if(QueryPerformanceCounter(&liStart))           
                {
                    // Do Stuff
                    runThreads(threadNum);
                    // Get Time spent...
                    LARGE_INTEGER liStop = {0};
                    if(QueryPerformanceCounter(&liStop))    
                    {               
                        LONGLONG duration = (LONGLONG)((liStop.QuadPart - liStart.QuadPart) * 1000000000.0 / liFrequency.QuadPart);
                        durations[round] = duration;
                        fprintf(stderr, "Run %d with %d: %lld nanosec\n", round, threadNum, duration);
                    } // end if
                } // end if
            } // end if
        } // end for round 
        double average = get_average(durations, AVERAGE_RUN); 
        double standardDeviation = std(durations, AVERAGE_RUN, average); 
        printf("%d, %f, %f\n", threadNum, average, standardDeviation);
    } // end for threadNum 

    // exit
    return 0;
} // end main
