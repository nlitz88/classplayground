#include "heap.h"
#include "stdlib.h"

int test_heap_new() {
    heap_t *heap = heap_new();
    if (heap != NULL) {
        return 0;
    }
    return 1;
}

int main() {

    test_heap_new();
    return 0;
}