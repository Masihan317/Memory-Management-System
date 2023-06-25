/**
 * Author: Sihan Ma
 * UWNetID: sihanm2
 * CSE 374 Homework 6A
 * Date: 05.13.2023
 * Copyright 2023 Sihan Ma/UW CSE374. All Rights Reserved.
 * Modified Starter Code Provided.
 *
 * bench.c is the benchmarking/test program for mem memory management.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"
#include <getopt.h>

/**
 * print_stats is a helper function that prints performance
 * information about the memory system.  Requires a clock_t start
 * time, and prints elapsed time plus results from get_mem_stats()
 */
void print_stats(clock_t start);

/**
 * fill_mem is a helper function that takes in an address and the
 * size of the memory allocation at that address.  It should fill
 * the first 16 bytes (or up to size) with hexedecimal values.
 */
void fill_mem(void* ptr, uintptr_t size);

/**
 * Main function of the program (bench test).
 * Synopsis: bench (main)
 * [ntrials] (10000) getmem + freemem calls
 * [pctget] (50) % of calls that are get mem
 * [pctlarge] (10) % of calls requesting more memory than lower limit
 * [small_limit] (200) largest size in bytes of small block
 * [large_limit] (20000) largest size in bytes of large block
 * [random_seed] (time) initial seed for rand
 * @param int argc - number of arguments
 * @param char **argv - array of arugments passed in
 * @returns int - whether the files executed successfully or failed
 */
int main(int argc, char** argv) {
  // initialize the parameters
  int NTRIALS = 10000;
  int PCTGET = 50;
  int PCTLARGE = 10;
  int SMALL_L = 200;
  int LARGE_L = 20000;
  int RANDOM_SEED = time(NULL);

  int param;  // variable to be used in the getopt switch statement

  // parses command line arguments
  // slight modified from the sample getopt code on The GNU C Library
  while ((param = getopt(argc, argv, "1:2:3:4:5:6:")) != -1) {
    switch (param) {
      case '1':
        NTRIALS = atoi(optarg);  // convert input string to int
        break;
      case '2':
        PCTGET = atoi(optarg);  // convert input string to int
        break;
      case '3':
        PCTLARGE = atoi(optarg);  // convert input string to int
        break;
      case '4':
        SMALL_L = atoi(optarg);  // convert input string to int
        break;
      case '5':
        LARGE_L = atoi(optarg);  // convert input string to int
        break;
      case '6':
        RANDOM_SEED = atoi(optarg);  // convert input string to int
        break;
      default:
        // input arguments do not correspond to an option
        fprintf(stderr, "Invalid argument.\n");
        return EXIT_FAILURE;
    }
  }

  srand(RANDOM_SEED);  // randomize seed

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  int ntrials = 0, nblocks = 0;
  clock_t start = clock();

  // perform NTRIALS mem operations
  for (int i = 0; i < NTRIALS; i++) {
    if ((rand() % 100) < PCTGET) {
      // allocate a block
      int size;
      if ((rand() % 100) < PCTLARGE) {
        // generate random size of large block
        size = SMALL_L + (rand() % (1 + LARGE_L - SMALL_L));
      } else {
        // generate random size of small block
        size = 1 + (rand() % SMALL_L);
      }

      blocks[nblocks] = getmem(size);  // allocate memory
      assert(blocks[nblocks]);  // check if memory allocation failed
      fill_mem(blocks[nblocks], size);  // fill memory with hex values
      nblocks++;  // increment block count
    } else {
      // free a previously acquired block
      if (nblocks != 0) {
        int random_index = rand() % nblocks;  // get a random index
        freemem(blocks[random_index]);  // free memory at that index
        // fill the freed index with the last element in block storage
        // got help from the TA on this one
        blocks[random_index] = blocks[nblocks - 1];
        nblocks--;  // decrement block count
      }
    }

    ntrials++;  // increment trial count

    // print statistics every 1/10th of the way through the loop
    if (NTRIALS < 10) {
      // prints every loop if the number of trials is less than 10
      print_stats(start);
    } else if (i % (NTRIALS / 10) == 0) {
      // got help from the TA on this one
      print_stats(start);
    }
  }

  return EXIT_SUCCESS;
}

/**
 * Helper function that prints performance information about the memory
 * system. Prints elapsed time from the given start time and results from
 * get_mem_stats() including total amount of storage, total number of
 * blocks, and average number of bytes
 * @param clock_t start - start time of the bench test
 */
void print_stats(clock_t start) {
  // calculate the amount of time elapsed between start and end
  // the following 2 lines of code are taken from GeeksForGeeks
  clock_t end = clock();
  double duration = (double) (end - start) / CLOCKS_PER_SEC;

  // initialize values to store the current statistics
  uintptr_t total_size = 0;
  uintptr_t total_free = 0;
  uintptr_t n_free_blocks = 0;

  // update values with the current statistics
  get_mem_stats(&total_size, &total_free, &n_free_blocks);

  // print out the relevant information
  fprintf(stdout, "Total CPU time used by the benchmark test so far in "
  "seconds: %f\n", duration);
  fprintf(stdout, "Total amount of storage acquired from the underlying "
  "system by the memory manager during the test so far: %lu\n", total_size);
  fprintf(stdout, "Total number of blocks on the free storage list at this "
  "point in the test: %lu\n", total_free);
  fprintf(stdout, "Average number of bytes in the free storage blocks at "
  "this point in the test: %lu\n", n_free_blocks);
}

/**
 * Helper function that fills the first 16 bytes (or up to size) of the
 * input address with hexedecimal values.
 * @param void* ptr - pointer to memory address to be filled
 * @param uintptr_t size - size of memory allocation at that address
 */
void fill_mem(void* ptr, uintptr_t size) {
  // cast pointer to type uintptr_t
  uintptr_t memadd = (uintptr_t) ptr;

  // fill memory with hex values
  for (int i = 0; i < 16 && i < size; i++) {
    *((unsigned char*) (memadd + i)) = 0xFE;
  }
}
