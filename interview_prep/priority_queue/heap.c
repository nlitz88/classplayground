#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

static const uint32_t HEAP_STEP = 32;

heap_response_t *allocate_heap_node_array(uint32_t size, heap_node_t **new_heap_node_array_pointer) {
    heap_node_t *new_heap_node_array = malloc(size*HEAP_STEP*sizeof(heap_node_t));
    if (new_heap_node_array == NULL) {
        return FAILURE;
    }
    *new_heap_node_array_pointer = new_heap_node_array;
    return SUCCESS;
}

heap_response_t expand_heap_node_array(uint32_t current_size, heap_node_t **new_heap_node_array) {
    heap_node_t *new_heap_node_array = NULL;
    heap_response_t response = allocate_heap_node_array(current_size+1, new_heap_node_array);
    if (response == FAILURE) {
        return FAILURE;
    }
    *new_heap_node_array = new_heap_node_array;
    return SUCCESS;
}

heap_t *heap_new() {
    heap_t *new_heap = malloc(sizeof(heap_t));
}

void heap_sift_down();

void heap_bubble_up();

