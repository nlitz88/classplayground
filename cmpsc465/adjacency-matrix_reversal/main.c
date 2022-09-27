#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

/* 
    Need some function to actually accept an adjancy matrix of some sort.
    Or, we'll just hardcode an example adjaceny matrix.
    MAYBE: we can accept an adjaceny matrix in the inefficient form of a 2D array. THEN, there can be a function
    that allows us to ingest that 2D array and produce a space efficient bitmap of sorts!
*/

/**
 * SO, let's think about how we can represent a 2D array in the form of a bitmap.
 * The smallest unit we have to work with is a byte == 8 bits (a character type).
 * Therefore, we'll have to first have a function to figure out the number of bytes needed to accomodate all of the
 * columns and then all of the rows.
 * 
 * That is, I think we should have as many bytes as we need to hold all of the entries for a row that we need need
 * (as an array), and then just have an array of those arrays for as many rows as we need.
 * 
 * This design means that for graphs with |v| % 8 != 0, we'll have some wasted bits at the end of each row. Is
 * there any way around that? I can't really think of anything. Even with that waste, it's still more space
 * efficient than a whole on 2D array.
 * 
*/

typedef uint64_t vertexcounter;

/**
 * @brief This type used to hold the number of blocks needed to represent the number of vertices in a graph. That
 * is, if a graph has 65 vertices, we'll need 65/sizeof(adjblk)*8 + 65%(sizeof(adjblk)*8) adjblocks == 9 adjblks to
 * represent 65 vertices, as each one can hold 8 vertices! Assuming the size of an adjblk is 8 bits! Being 64 bits,
 * that means this type will support representation of up to 2^64 quantity of adjblks, which corresponds to a
 * maximum of (2^64)*8 vertices that can be represented! We'll also use this to count the number of rows!
 */
typedef uint64_t blockcounter;
typedef uint8_t adjblk;
// typedef adjblk *adjblkrow;

/**
 * @brief A matrix is the data structure that we'll use to efficiently represent the adjacency matrix of a provided
 * graph. It is effectively also a 2D array, but an array of arrays, where each of the arrays is an array of
 * characters, where each bit represents adjacency of a vertex to another.
 */
typedef adjblk **matrix;

static inline vertexcounter matrix_get_req_row_adjblks(vertexcounter num_vertices) {

    // Cool trick: Basically, we add one if there is overflow, as it'll require one more adjblk to represent.    
    return num_vertices / (sizeof(adjblk)*8) + (num_vertices % (sizeof(adjblk)*8) != 0);
    
}

static inline bool test_matrix_get_req_row_adjblks() {
    
    assert(matrix_get_req_row_adjblks(65) == 9);
    assert(matrix_get_req_row_adjblks(6) == 1);
    printf("All tests passed!\n");
    return true;
}

// NOTE: Don't need a function to compute the number of adjacency block rows we'll need, as this is just number of
// vertices!

/**
 * @brief Allocate memory for the new adjacency matrix.
 * 
 * @param num_vertices The number of vertices in the graph.
 */
static inline matrix matrix_allocate(vertexcounter num_vertices) {

    // Get the number of required adjacency blocks per row.
    vertexcounter num_row_adj_blks = matrix_get_req_row_adjblks(num_vertices);
    // Allocate the space for each row and create the matrix data structure.
    // First, we're basically just allocating the outer array, which should just be an array of pointers to
    // adjacency blocks. Therefore, we want to allocate, for each, the amount of memory we need for each of our
    // pointers to adjacency blocks.
    // adj_matrix will hold the address of the first pointer to the first adjblk of the first row.
    matrix adj_matrix = malloc(num_vertices*sizeof(adjblk *));

    // Then, from there, we need to allocate the memory needed for each adjacency block row.
    for (int r = 0; r < num_vertices; ++r) {
        // For each adjacency block row, allocate memory for num_row_adj_blks for that row.
        // Use calloc so that these allocations are all initially zeroed out!
        adj_matrix[r] = calloc(num_row_adj_blks, sizeof(adjblk));
    }

    return adj_matrix;

}

/**
 * @brief Returns which adjblk the provided column (x) falls into. This calculation is literally just the provided
 * column divided by the size of the adjaceny block!
 * 
 * @param col The x value or the column.
 * @return vertexcounter: The adjblk that the column falls into.
 */
static inline vertexcounter adjblk_from_col(vertexcounter col) {
    return col / sizeof(adjblk);
}

/**
 * @brief This funciton returns the number of the bit inside of an adjacency block based on the provided column.
 * This calculation is literally just the column mod the size of an adjacency block!
 * 
 * @param col 
 * @return uint8_t 
 */
static inline uint8_t adjblk_bit_pose_from_col(vertexcounter col) {
    return col % sizeof(adjblk_from_col);
}

/**
 * @brief This function sets the value of the element at the xth row, yth column to the provided value.
 * 
 * @param matrix Matrix to update. This is okay to just be a copy of the pointer, as this function doesn't modify
 * the matrix pointer itself at all.
 * @param x Row the element is in.
 * @param y Column the element is in.
 * @param value Value can be true/false == 1/0.
 */
static inline void matrix_set_element(matrix matrix, vertexcounter x, vertexcounter y, bool value) {

    // So, the first step is to build up this bitmask to adjust the modified value. Basically, we want to AND out
    // the bit at the given position.
    uint8_t adjblk_bit_pos = adjblk_bit_pose_from_col(x);
    // To build our mask, we'll take a 1 and shift it to the position that the bit will be in, then take the
    // COMPOF THE WHOLE MASK -- which will give us 0 surrounded by ones. Bitwise AND this with the current value
    // and we've successfully masked out whatever value was already at that bit position.
    adjblk mask = ~(0x1 << (sizeof(adjblk) - 1 - adjblk_bit_pos));
    // Get the adjacency block that we're actually updating and apply the mask.
    vertexcounter adjblk_num = adjblk_from_col(x);
    adjblk *curr_adjblk = &(matrix[y][adjblk_num]);
    *curr_adjblk = *curr_adjblk & mask;

    // Now, take the value and shift it to the proper position, then bitwise OR it with the newly masked existing
    // adjblk.
    adjblk new_adjblk_value = ((adjblk)value & 0x1) << (sizeof(adjblk) - 1 - adjblk_bit_pos);        // Ensures we only get the LSB == Last bit on the right.
    *curr_adjblk = *curr_adjblk | new_adjblk_value;

    return;

}

/**
 * @brief This function returns the value at the xth row, yth column.
 * 
 * @param adj_matrix 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
static inline bool matrix_get_element(matrix adj_matrix, vertexcounter x, vertexcounter y) {

    // First, get the adjency block from the column.
    vertexcounter adjblk_num = adjblk_from_col(x);
    // Use this to get the actual adjency block that we're working on.
    adjblk *curr_adjblk = &(adj_matrix[y][adjblk_num]);
    // Also get the position within the adjacency block.
    uint8_t adjblk_bit_pos = adjblk_bit_pose_from_col(x);

    // Now, create a bitmask to extract the value from the adjblk num. Just place a 1 amidst a bunch of zeroes at
    // the proper position, bitwise AND it with the adjblk, then shift the value to the right (we don't even need
    // to shift it in this case, as anything nonzero is true).
    adjblk mask = 0x1 << (sizeof(adjblk) - 1 - adjblk_bit_pos);
    return (bool)(*curr_adjblk & mask);

}

static inline void test_matrix_get_element() {

    matrix adj_matrix = matrix_allocate(10);
    matrix_set_element(adj_matrix, 1, 1, true);
    assert(matrix_get_element(adj_matrix, 1, 1) == 1);
    matrix_set_element(adj_matrix, 1, 1, false);
    assert(matrix_get_element(adj_matrix, 1, 1) == 0);

}


int main() {

// #ifdef TEST
    test_matrix_get_req_row_adjblks();
    test_matrix_get_element();
// #endif

    // matrix adj_matrix = matrix_allocate(10);
    // matrix_set_element(adj_matrix, 1, 1, true);
    // matrix_set_element(adj_matrix, 1, 1, false);

    return 0;
}