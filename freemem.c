/**
 * Author: Sihan Ma
 * Date: 05.18.2023
 * Copyright 2023 Sihan Ma/UW CSE374. All Rights Reserved.
 * Modified Starter Code Provided.
 *
 * Implements freemem (free) for memory system. Puts the block of memory
 * allocated from getmem back to the freelist for reuse.
 */

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

// inserts a node in between 2 nodes (previous and current)
void insert_node(freeNode* previous, freeNode* node, freeNode* current);

// merges 2 nodes into a single node
void merge_node(freeNode* first, freeNode* second);

/**
 * Return the block of storage at location p to the pool of available
 * free storage.
 * @param void* p - the pointer value of the location of storage returned
 */
void freemem(void* p) {
  // check for possible problems with freelist
  check_heap();

  // return immediately if p or freelist is NULL
  if (p == NULL || freelist == NULL) {
    return;
  }

  // cast the pointer value to a node
  freeNode* node = (freeNode*) ((uintptr_t) p - NODESIZE);

  // loop through the freelist
  freeNode* previous = freelist;
  freeNode* current = freelist;
  while (current != NULL) {
    // found the place to insert the node
    if (node < current) {
      insert_node(previous, node, current);
      break;  // exit the loop once the insert location is found
    }
    previous = current;
    current = current->next;
  }

  // insert at the end of the list
  if (current == NULL) {
    insert_node(previous, node, NULL);
  }
}

/**
 * Inserts a node in between 2 nodes (previous and current).
 * @param freeNode* previous - the node that should be right before node
 * after node insertion
 * @param freeNode* node - the node to be inserted
 * @param freeNode* current - the node that should be right after node
 * afternode insertion
 */
void insert_node(freeNode* previous, freeNode* node, freeNode* current) {
  // rearranges pointers depending on if node will be the first node in list
  if (previous == current) {
    node->next = current;
    freelist = node;
  } else {
    node->next = current;
    previous->next = node;
  }

  // check if right side of node can be merged
  // only checks if node is not inserted as the last node
  if ((node->next != NULL)
  && (node->size + NODESIZE + ((uintptr_t) node)
  == ((uintptr_t) current))) {
    merge_node(node, current);
  }

  // check if left side of node can be merged
  // onky checks if node is not inserted as the first node
  if ((previous != current)
  && (previous->size + NODESIZE + ((uintptr_t) previous)
  == ((uintptr_t) node))) {
    merge_node(previous, node);
  }

  // check for possible problems with freelist
  check_heap();
}

/**
 * Merges 2 nodes first and second into a single node.
 * @param freeNode* first - the first node to be merged
 * @param freeNode* second - the second node to be merged
 */
void merge_node(freeNode* first, freeNode* second) {
  first->size += (NODESIZE + second->size);
  first->next = second->next;
}
