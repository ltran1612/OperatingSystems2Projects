# Program to spawn from 1 to MAX_THREADS_RUN number of threads
# Then fill an array 1 << 31 times. 
# Each thread number is run five times.
# The run time of each run is printed to STDERR
# The program will also calculate the average and standard deviation. 
# Then, it will be printed out on STDOUT. 
# Author: Long Tran
# Date: Feb 10, 2024
import numpy as np
import threading
import time 
import sys

MAX_THREADS_RUN = 32 
AVERAGE_RUN = 5

def my_thread_func():
    length = 1 << 20; 
    array  = [i for i in range(10)]
    for i in range(length): 
        array[i % 10] = 0

    for i in range(10):
        array[0] += array[i]

def run_threads(thread_num):
    threads = [i for i in range(thread_num)]
    for i in range(thread_num):
        threads[i] = threading.Thread(target=my_thread_func)
        threads[i].start()
    
    for thread in threads: 
        thread.join()

if __name__ == "__main__":
    for threadNum in range(1, MAX_THREADS_RUN+1, 1):
        durations = []
        for round in range(AVERAGE_RUN):
            start = time.perf_counter()
            run_threads(threadNum)
            end = time.perf_counter() 
            duration = (end - start) * 1000000000

            durations.append(duration)

            print(f"Run {round} with {threadNum}: {duration} nanosec", file=sys.stderr)
        average = np.average(durations) 
        std= np.std(durations, ddof=1) 
        print(f"{threadNum}, {average}, {std}")
