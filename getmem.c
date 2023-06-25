/**
 * Author: Sihan Ma
 * Date: 05.18.2023
 * Copyright 2023 Sihan Ma/UW CSE374. All Rights Reserved.
 * Modified Starter Code Provided.
 *
 * Implements getmem (malloc) for memory system. The user can request memory
 * and the program will return a block of storage at least the requested size.
 */

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

// initializes extern global variable
freeNode* freelist = NULL;
uintptr_t totalmalloc = 0;

// allocates size bytes of memory and return a pointer to it
freeNode* allocate_memory(uintptr_t size);

// splits the current block of memory into 2 with the first block
// having size bytes of available usable memory
void split_node(freeNode* previous, freeNode *current, uintptr_t size);

/**
 * Return a pointer to a new block of storage with at least size bytes
 * of memory.
 * @param uintptr_t size - size of memory the user requested in bytes
 * @returns void* - pointer to allocated memory that the user can use
 */
void* getmem(uintptr_t size) {
  // check for possible problems with freelist
  check_heap();

  // check if size is valid (a positive integer)
  if (size <= 0) {
    return NULL;
  }

  // create a new memory block of size BIGCHUNK if nothing is in the list
  if (freelist == NULL) {
    freelist = allocate_memory(BIGCHUNK);
  }

  // if size is smaller than MINCHUNK, round it up to MINCHUNK
  if (size < MINCHUNK) {
    size = MINCHUNK;
  }

  // if size is not a multiple of 16, round it up to the nearest multiple of 16
  if (size % 16 != 0) {
    size += (16 - size % 16);
  }

  // loop through freelist to find a chunk of the right size
  freeNode* previous = NULL;
  freeNode* current = freelist;
  while (current != NULL) {
    // a block large enough is found
    if (current->size > size) {
      if (current->size > (size + NODESIZE + MINCHUNK)) {
        // split the node if the node is big enough
        split_node(previous, current, size);
      } else {
        // rearranges the pointers depending on if the node is at front of list
        if (previous == NULL) {
          freelist = current->next;
        } else {
          previous->next = current->next;
        }
      }
      check_heap();  // check for possible problems with freelist
      // return the current block of usable memory
      return (void*) ((uintptr_t) current + NODESIZE);
    }
    previous = current;
    current = current->next;
  }

  check_heap();  // check for possible problems with freelist
  // allocate memory if no available memory is big enough on the freelist
  return (void*) ((uintptr_t) (allocate_memory(size)) + NODESIZE);
}

/**
 * Allocates size bytes of memory and returns a pointer to that memory.
 * @param uintptr_t size - size of memory requested in bytes
 * @returns freeNode* - pointer to allocated memory of size bytes
 */
freeNode* allocate_memory(uintptr_t size) {
  freeNode* node = (freeNode*) malloc(NODESIZE + size);
  assert(node);  // check if memory allocation failed
  node->size = size;
  node->next = NULL;
  totalmalloc += (NODESIZE + size);  // updates information on memory allocation
  return node;
}

/**
 * Splits the current block of memory into 2 with the first block having
 * size bytes of available usable memory.
 * @param freeNode* previous - the node before the current node
 * @param freeNode* current - the current node to split in 2 nodes
 * @param uintptr_t size - size of available memory in the first node
 * of the 2 nodes split from the current node
 */
void split_node(freeNode* previous, freeNode* current, uintptr_t size) {
  // creates a new node and updates the size of the 2 split node
  freeNode* node = (freeNode*) ((uintptr_t) current + NODESIZE + size);
  node->size = current->size - NODESIZE - size;
  node->next = current->next;
  current->size = size;
  // rearranges the pointers depending on if the node is at front of list
  if (previous == NULL) {
    freelist = node;
  } else {
    previous->next = node;
  }
}
