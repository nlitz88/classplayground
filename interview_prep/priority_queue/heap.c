#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

static const uint32_t HEAP_STEP = 32;

heap_response_t allocate_heap_node_array(uint32_t size, heap_node_t **new_heap_node_array_pointer) {
    heap_node_t *new_heap_node_array = malloc(size*HEAP_STEP*sizeof(heap_node_t));
    if (new_heap_node_array == NULL) {
        return FAILURE;
    }
    *new_heap_node_array_pointer = new_heap_node_array;
    return SUCCESS;
}

heap_response_t expand_heap_node_array(uint32_t current_size, heap_node_t **heap_node_array_pointer) {
    heap_node_t *new_heap_node_array = NULL;
    /** Allocate new, larger array. */
    heap_response_t response = allocate_heap_node_array(current_size+1, &new_heap_node_array);
    if (response == FAILURE) {
        return FAILURE;
    }
    /** Copy elements from old array to new array. Each element is just the
     * address of a heap node, so can just copy the value of those over. */
    for (int i=0; i < current_size*HEAP_STEP; ++i) {
        new_heap_node_array[i] = (*heap_node_array_pointer)[i];
    }
    /** De-allocate the memory associated with the old heap node array. */
    free(*heap_node_array_pointer);
    /** Set the address of the heap_node_array_pointer equal to the new array's
     * address. */
    *heap_node_array_pointer = new_heap_node_array;
    return SUCCESS;
}

heap_t *heap_new() {
    heap_t *new_heap = malloc(sizeof(heap_t));
    allocate_heap_node_array(1, &(new_heap->node_array));
}

void heap_sift_down(heap_t *heap) {

}

void heap_bubble_up(heap_t *heap) {



}

