#include <stdio.h>
#include "myalloc.h"
#include "list.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]) {
    // First fit
    initialize_allocator(100, FIRST_FIT);
    printf("Using first fit algorithm on memory size 100\n");

    // Best fit
    // initialize_allocator(100, BEST_FIT);
    // printf("Using best fit algorithm on memory size 100\n");

    // Worse fit
    // initialize_allocator(100, WORST_FIT);
    // printf("Using worse fit algorithm on memory size 100\n");

    int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        p[i] = allocate(sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    print_statistics();

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        deallocate(p[i]);
        p[i] = NULL;
    }

    printf("available_memory %d\n", available_memory());

    void* before[100] = {NULL};
    void* after[100] = {NULL};
    compact_allocation(before, after);

    print_statistics();

    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit

    destroy_allocator();

    return 0;
}




















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Additional testing: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// int main(int argc, char* argv[]) {

    // Testing code: chunk1 
    // Initialize the allocator
    /*initialize_allocator(1024, FIRST_FIT);

    // Run tests
    test_allocate_first_fit();
    test_allocate_best_fit();
    test_allocate_worst_fit();
    test_deallocate();
    test_compact_allocation();
    // test_print_statistics();

    // Cleanup
    destroy_allocator();*/



    // Testing code: chunk2
    // Initialize allocator
    // initialize_allocator(2048, FIRST_FIT);

    // Run additional tests
    /*test_allocate_large_request();
    test_allocate_multiple_small_blocks();
    test_allocate_exact_fit();
    test_deallocate_nonexistent();
    test_allocate_after_compaction();
    test_repeated_allocation_and_deallocation();
    test_edge_case_allocation();
    test_fragmentation();
    test_compact_with_fragmentation();

    // Cleanup
    destroy_allocator();*/

//     return 0;
// }

// Testing code: chunk1 

/*void test_allocate_first_fit() {
    printf("Testing First Fit Allocation...\n");
    void* ptr1 = allocate(200);
    assert(ptr1 != NULL);
    void* ptr2 = allocate(100);
    assert(ptr2 != NULL);
    void* ptr3 = allocate(50);
    assert(ptr3 != NULL);

    // Ensure allocated blocks
    assert(available_memory() == 674); // 1024 - (200 + 100 + 50) = 674

    print_statistics();
}

void test_allocate_best_fit() {
    printf("Testing Best Fit Allocation...\n");
    destroy_allocator();
    initialize_allocator(1024, BEST_FIT);

    void* ptr1 = allocate(200);
    assert(ptr1 != NULL);
    void* ptr2 = allocate(100);
    assert(ptr2 != NULL);
    void* ptr3 = allocate(50);
    assert(ptr3 != NULL);

    // Ensure allocated blocks
    assert(available_memory() == 674); // 1024 - (200 + 100 + 50) = 674

    print_statistics();
}

void test_allocate_worst_fit() {
    printf("Testing Worst Fit Allocation...\n");
    destroy_allocator();
    initialize_allocator(1024, WORST_FIT);

    void* ptr1 = allocate(200);
    assert(ptr1 != NULL);
    void* ptr2 = allocate(100);
    assert(ptr2 != NULL);
    void* ptr3 = allocate(50);
    assert(ptr3 != NULL);

    // Ensure allocated blocks
    assert(available_memory() == 674); // 1024 - (200 + 100 + 50) = 674

    print_statistics();
}

void test_deallocate() {
    printf("Testing Deallocation...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(200);
    void* ptr2 = allocate(100);
    void* ptr3 = allocate(50);

    // Deallocate some blocks
    deallocate(ptr2);
    deallocate(ptr3);

    print_statistics();
}

void test_compact_allocation() {
    printf("Testing Compaction...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(200);
    void* ptr2 = allocate(100);
    deallocate(ptr1);
    deallocate(ptr2);

    void* before[10];
    void* after[10];
    int compactedSize = compact_allocation(before, after);

    // Verify compaction
    assert(compactedSize > 0);
    assert(available_memory() == 1024); // All space should be available after compaction

    print_statistics();
}
*/


// Testing code: chunk2

/*void test_allocate_large_request();
void test_allocate_multiple_small_blocks();
void test_allocate_exact_fit();
void test_deallocate_nonexistent();
void test_allocate_after_compaction();
void test_repeated_allocation_and_deallocation();
void test_edge_case_allocation();
void test_fragmentation();
void test_compact_with_fragmentation();


// Test: Allocate a large request that exceeds available memory
void test_allocate_large_request() {
    printf("Testing Allocation of Large Request...\n");
    void* ptr = allocate(2048);
    // assert(ptr == NULL); // Allocation should fail as it exceeds available memory

    print_statistics();
}

// Test: Allocate multiple small blocks
void test_allocate_multiple_small_blocks() {
    printf("Testing Allocation of Multiple Small Blocks...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptrs[10];
    for (int i = 0; i < 10; ++i) {
        ptrs[i] = allocate(50); // Allocate 10 blocks of 50 bytes each
        assert(ptrs[i] != NULL);
    }

    print_statistics();
}

// Test: Allocate blocks that exactly fit the remaining free space
void test_allocate_exact_fit() {
    printf("Testing Allocation with Exact Fit...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(300);
    void* ptr2 = allocate(300);
    void* ptr3 = allocate(424); // This should fit exactly into the remaining space
    assert(ptr3 != NULL);

    print_statistics();
}

// Test: Deallocate a pointer that was not allocated
void test_deallocate_nonexistent() {
    printf("Testing Deallocation of Nonexistent Block...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr = allocate(100);
    void* invalid_ptr = (char*)ptr + 10; // An invalid pointer for deallocation
    deallocate(invalid_ptr); // Should handle this gracefully

    print_statistics();
}

// Test: Allocate after compaction
void test_allocate_after_compaction() {
    printf("Testing Allocation After Compaction...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(200);
    void* ptr2 = allocate(100);
    deallocate(ptr1);
    deallocate(ptr2);

    // Compact memory
    void* before[10];
    void* after[10];
    int compactedSize = compact_allocation(before, after);
    assert(compactedSize > 0);

    // Allocate after compaction
    void* ptr3 = allocate(300); // Should succeed as we compacted the memory
    assert(ptr3 != NULL);

    print_statistics();
}

// Test: Repeated allocation and deallocation
void test_repeated_allocation_and_deallocation() {
    printf("Testing Repeated Allocation and Deallocation...\n");
    destroy_allocator();
    initialize_allocator(2048, FIRST_FIT);

    for (int i = 0; i < 100; ++i) {
        void* ptr = allocate(20);
        assert(ptr != NULL);
        deallocate(ptr);
    }

    print_statistics();
}

// Test: Edge cases with very small allocations
void test_edge_case_allocation() {
    printf("Testing Edge Case Allocations...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(1); // Allocate very small block
    void* ptr2 = allocate(1); // Allocate another small block
    void* ptr3 = allocate(1022); // Allocate the rest of the memory

    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr3 != NULL);

    deallocate(ptr1);
    deallocate(ptr2);
    deallocate(ptr3);

    print_statistics();
}

// Test: Fragmentation test
void test_fragmentation() {
    printf("Testing Fragmentation...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(300);
    void* ptr2 = allocate(200);
    deallocate(ptr1);

    // Allocate in a way that causes fragmentation
    void* ptr3 = allocate(100);
    void* ptr4 = allocate(50);
    void* ptr5 = allocate(50);

    assert(ptr3 != NULL);
    assert(ptr4 != NULL);
    assert(ptr5 != NULL);

    print_statistics();
}

// Test: Compaction with fragmentation
void test_compact_with_fragmentation() {
    printf("Testing Compaction with Fragmentation...\n");
    destroy_allocator();
    initialize_allocator(1024, FIRST_FIT);

    void* ptr1 = allocate(300);
    void* ptr2 = allocate(200);
    deallocate(ptr1);
    deallocate(ptr2);

    // Allocate with fragmentation
    void* ptr3 = allocate(100);
    void* ptr4 = allocate(50);
    void* ptr5 = allocate(50);

    assert(ptr3 != NULL);
    assert(ptr4 != NULL);
    assert(ptr5 != NULL);

    // Compact memory
    void* before[10];
    void* after[10];
    int compactedSize = compact_allocation(before, after);
    assert(compactedSize > 0);

    print_statistics();
}
*/