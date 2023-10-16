/**
 * Heap implementation over an array of heap nodes, where each heap node
 * contains a generic pointer to a struct instance and a priority number.
 * 
 * The heap implementation is responsible for providing functions to 
 * 1.) heapify an existing array of heap_nodes_t.
 * 2.) Allocate a new, empty heap.
 * 3.) for inserting new heap_node_t instances into the heap.
 * For now, let's say they provide the node_data, and then they also have to
 * provide a function pointer to the function that will "get" a priority out of
 * that node's data. This is fine, but every insertion then is going to require
 * this function pointer. Is that reasonable? We could roll with it as an
 * experiment.
 * 4.) for deleting an existing heap_node_t instance from the heap (given the
 * node's address--the one that would be found in the heap 
 * array).
 * 5.) for deallocating an entire heap (but not the underlying data elements
 * referred to by the heap).
 * 
*/

/**
 * @brief type definition for the struct type that will serve as the node for
 * the elements of the heap.
 * 
 * node_data: pointer to the data instance in memory.
 * get_priority_function: function pointer to the function that will take the
 * node_data instance referred to by node_data and extract a priority value from
 * it.
 * 
 */
typedef struct {
    void *node_data;
    void (*get_priority_function)(void *);
} heap_node_t;

/**
 * @brief type definition for the struct type that will serve as the main
 * interface to the underlying heap.
 * 
 * node_array: pointer to the first node in an array of heap nodes.
 * 
 */
typedef struct {
    heap_node_t *node_array;
} heap_t;

typedef enum {
    SUCCESS = 0,
    FAILURE = 1
} heap_response_t;

/**
 * @brief Initializes a new heap.
 * 
 * @return the address of the new, initialized heap instance with the root node
 * pointer initialized to NULL.
 */
heap_t *heap_new();

/**
 * @brief Function to de-allocate the provided heap instance.
 * 
 * @param address of the heap to be de-allocated.
 */
void heap_destroy(heap_t *heap);

/**
 * @brief Function to insert a node into the heap. Provide the address of the
 * data instance that will be referenced and a pointer to a function the heap
 * will use to get a priority from the provided data instance.
 * 
 * @param node_data Address of the data object to be maintained in the heap.
 * @param get_priority_function Address of function that will be used to extract
 * a "priority" value from the provided data instance.
 */
void heap_insert(void *node_data, void (*get_priority_function)(void *));

/**
 * @brief Function to pop the node at the top of the heap off and return its
 * data value's address.
 * 
 * @return void* Returns the address of the data field referenced by the node at
 * the top of the heap.
 */
void *heap_pop();