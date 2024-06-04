// Program to spawn from 1 to MAX_THREADS_RUN number of threads
// Then fill an array 1 << 31 times. 
// Each thread number is run five times.
// The run time of each run is printed to STDERR
// The program will also calculate the average and standard deviation. 
// Then, it will be printed out on STDOUT. 
// Author: Long Tran
// Date: Feb 10, 2024
import java.util.Arrays;

public class Main {
    final static int MAX_THREADS_RUN = 32;
    final static int AVERAGE_RUN = 5;

    public static class MyRunnable implements Runnable {
        public void run() {
            int length = 1 << 20; 
            int[] array  = new int[10];
            for (int i = 0; i < length; ++i) {
                array[i % 10] = 0;
            } // end for i

            for (int i = 0; i < 10; ++i) {
                array[0] += array[i];
            } // end for i
        } // end run
    } // end MyThreadk

    public static void runThreads(int threadNum) {
        Thread[] threads = new Thread[threadNum];
        for (int i = 0; i < threadNum; ++i) {
            threads[i] = new Thread(new MyRunnable());
            try {
                threads[i].start();
            } catch(IllegalThreadStateException e) {
                System.err.println(e.getMessage());
                System.exit(1);
            } // end catch
        } // end for i

        for (int i = 0; i < threadNum; ++i) {
            try {
                threads[i].join();
            } catch(InterruptedException e) {
                System.err.println(e.getMessage());
                System.exit(1);
            } // end catch
        } // end 
    } // end runThreads

    public static void main(String[] args) {
        for (int threadNum = 1; threadNum <= Main.MAX_THREADS_RUN; threadNum++) {
            long[] durations = new long[Main.AVERAGE_RUN];
            for (int round = 0; round < Main.AVERAGE_RUN; ++round) {
                
                long start = System.nanoTime();
                // run the process
                runThreads(threadNum);
                long end = System.nanoTime();
                long duration = end - start;

                durations[round] = duration;

                System.err.printf("Run %d with %d: %d nanosec\n", round, threadNum, duration);
            } // end for round 
            double average = Arrays.stream(durations).average().getAsDouble();
            double standard_deviation= std(durations, average);
            System.out.printf("%d, %f, %f\n", threadNum, average, standard_deviation);
        } // end for threadNum 
    } // end main

    public static double std(long[] values, double mean) {
        double sum = 0;
        for (long value: values) {
            double diff = (double) (value) - mean;
            diff = diff * diff;
            diff = diff / (values.length-1);
            sum += diff;
        } // end for

        return Math.sqrt(sum);
    } // end std
} // en Main
