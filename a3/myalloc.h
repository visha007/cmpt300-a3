#ifndef __MYALLOC_H__
#define __MYALLOC_H__

enum AllocationAlgorithm { 
    FIRST_FIT, 
    BEST_FIT, 
    WORST_FIT 
};

void initialize_allocator(int size, enum AllocationAlgorithm algorithm);
void* allocate(int size);
void deallocate(void* ptr);
int available_memory();
void print_statistics();
int compact_allocation(void** before, void** after);
void destroy_allocator();

#endif
