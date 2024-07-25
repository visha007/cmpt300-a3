#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myalloc.h"
#include "list.h"

struct Myalloc {
    enum AllocationAlgorithm algorithm;
    int size;
    void* memory;
    struct Node* allocatedBlocks;
    struct Node* freeBlocks;
    int available;
    int used;
};
struct Myalloc allocator;

void initialize_allocator(int size, enum AllocationAlgorithm algorithm) {
    assert(size > 0);
    allocator.algorithm = algorithm;
    allocator.size = size;
    allocator.memory = malloc((size_t)allocator.size);
    allocator.allocatedBlocks = NULL;
    listInsertHead(&allocator.freeBlocks, listCreateNode(allocator.memory, size));
    allocator.available = size;
    allocator.used = 0;
}

void destroy_allocator() {
    free(allocator.memory);
    listFreeList(&allocator.allocatedBlocks);
    listFreeList(&allocator.freeBlocks);
}

void* allocate_first_fit(int size) {
    void* ptr = NULL;
    struct Node* curFree = allocator.freeBlocks;
    struct Node* chosen = NULL;

    while (curFree != NULL) {
        if (curFree->size >= size) {
            chosen = curFree;
            break;
        }
        curFree = curFree->next;
    }

    if (chosen != NULL) {
        ptr = chosen->block;
        if (chosen->size > size) {
            struct Node* remainder = listCreateNode(chosen->block + size, chosen->size - size);
            listInsertTail(&allocator.freeBlocks, remainder);
        }
        listDeleteNode(&allocator.freeBlocks, chosen);
    }

    return ptr;
}

void* allocate_best_fit(int size) {
    void* ptr = NULL;
    struct Node* curFree = allocator.freeBlocks;
    struct Node* chosen = NULL;
    int minSize = allocator.size + 1;

    while (curFree != NULL) {
        if (curFree->size >= size && curFree->size < minSize) {
            chosen = curFree;
            minSize = curFree->size;
        }
        curFree = curFree->next;
    }

    if (chosen != NULL) {
        ptr = chosen->block;
        if (chosen->size > size) {
            struct Node* remainder = listCreateNode(chosen->block + size, chosen->size - size);
            listInsertTail(&allocator.freeBlocks, remainder);
        }
        listDeleteNode(&allocator.freeBlocks, chosen);
    }

    return ptr;
}

void* allocate_worst_fit(int size) {
    void* ptr = NULL;
    struct Node* curFree = allocator.freeBlocks;
    struct Node* chosen = NULL;
    int maxSize = 0;

    while (curFree != NULL) {
        if (curFree->size >= size && curFree->size > maxSize) {
            chosen = curFree;
            maxSize = curFree->size;
        }
        curFree = curFree->next;
    }

    if (chosen != NULL) {
        ptr = chosen->block;
        if (chosen->size > size) {
            struct Node* remainder = listCreateNode(chosen->block + size, chosen->size - size);
            listInsertTail(&allocator.freeBlocks, remainder);
        }
        listDeleteNode(&allocator.freeBlocks, chosen);
    }

    return ptr;
}

void* allocate(int size) {
    // if size needed larger than available memory
    if ((size) > allocator.available)
        return NULL;

    void* ptr = NULL;

    switch (allocator.algorithm) {
        case FIRST_FIT:
            ptr = allocate_first_fit(size);
            break;
        case BEST_FIT:
            ptr = allocate_best_fit(size);
            break;
        case WORST_FIT:
            ptr = allocate_worst_fit(size);
            break;
        default:
            printf("Unsupported allocation algorithm\n");
            break;
    }

    /*if (ptr != NULL) {
        struct Node* block = listCreateNode(ptr, size);
        listInsertTail(&allocator.allocatedBlocks, block);
        allocator.available -= size;
        allocator.used += size;
    }*/

    if (ptr != NULL) {
        // Create a new node for the allocated block
        struct Node* block = listCreateNode(ptr, size);

        // Insert the new node into the allocated list
        if (allocator.allocatedBlocks == NULL) {
            allocator.allocatedBlocks = block;
        } else {
            struct Node* temp = allocator.allocatedBlocks;
            listInsertTail(&allocator.allocatedBlocks, block);
        }

        allocator.available -= size;
        allocator.used += size;
    }

    return ptr;
}

/*void deallocate(void* ptr) {
    assert(ptr != NULL);
    struct Node* target = listFindNode(allocator.allocatedBlocks, ptr);
    struct Node* freeBlock = listCreateNode(target->block, target->size);

    mergeFreeBlocks(freeBlock);

    allocator.available += target->size;
    allocator.used -= target->size;
    listDeleteNode(&allocator.allocatedBlocks, target);
}*/

void deallocate(void* ptr) {
    if (ptr == NULL) 
        return;

    // Adjust the pointer to get the original memory block start (including header)
    // void* memory_block = ptr - sizeof(size_t);

    struct Node* target = listFindNode(allocator.allocatedBlocks, ptr);
    if (target == NULL) {
        printf("Attempt to deallocate a non-allocated block! Exiting program!\n");
        return;
    }

    struct Node* freeBlock = listCreateNode(target->block, target->size);
    mergeFreeBlocks(freeBlock);

    // Update allocator's available and used memory
    allocator.available += target->size;
    allocator.used -= target->size;

    // Remove the target node from the allocated blocks list
    listDeleteNode(&allocator.allocatedBlocks, target);
}

void mergeFreeBlocks(struct Node* block) {
    struct Node* cur = allocator.freeBlocks;
    struct Node* merged = listCreateNode(NULL, 0);
    struct Node* garbage[4] = { NULL };

    while (cur != NULL) {
        if ((cur->block + cur->size) == block->block && merged->block == NULL) {
            merged->block = cur->block;
            merged->size = cur->size + block->size;
            garbage[0] = cur;
        } else if ((cur->block + cur->size) == block->block && merged->block != NULL) {
            merged->block = cur->block;
            merged->size += cur->size;
            garbage[1] = cur;
        }
        if ((block->block + block->size) == cur->block && merged->block == NULL) {
            merged->block = block->block;
            merged->size = block->size + cur->size;
            garbage[2] = cur;
        } else if ((block->block + block->size) == cur->block && merged->block != NULL) {
            merged->size += cur->size;
            garbage[3] = cur;
        }
        cur = cur->next;
    }

    for (int i = 0; i < 4; ++i) {
        if (garbage[i] != NULL) {
            listDeleteNode(&allocator.freeBlocks, garbage[i]);
        }
    }

    if (merged->block != NULL) {
        listInsertTail(&allocator.freeBlocks, merged);
        free(block);
    } else {
        listInsertTail(&allocator.freeBlocks, block);
        free(merged);
    }
}

int compact_allocation(void** before, void** after) {
    int compactedSize = 0;

    store_allocation_state(before);

    if (allocator.used != 0) {
        compact_memory();
    }

    compactedSize = allocator.available;

    store_allocation_state(after);

    return compactedSize;
}

void compact_memory() {
    struct Node* curAlloc = allocator.allocatedBlocks;
    struct Node* curFree = allocator.freeBlocks;
    void* temp = NULL;

    if (allocator.used != 0) {
        memmove(allocator.memory, curAlloc->block, curAlloc->size);
        curAlloc->block = allocator.memory;

        if (curAlloc->next == NULL) {
            temp = curAlloc->block + curAlloc->size;
            memmove(temp, curFree->block, curFree->size);
            curFree->block = temp;
        } else {
            while (curAlloc->next != NULL) {
                memmove(curAlloc->block + curAlloc->size, curAlloc->next->block, curAlloc->size);
                curAlloc->next->block = curAlloc->block + curAlloc->size;
                temp = curAlloc->block + curAlloc->size;
                curAlloc = curAlloc->next;
            }
            if (allocator.available != 0) {
                memmove(temp + curAlloc->size, curFree->block, curFree->size);
                curFree->block = temp + curAlloc->size;
            }
        }

        while (curFree != NULL) {
            memmove(temp, curFree->block, curFree->size);
            curFree->block = temp;
            temp += curFree->size;
            curFree = curFree->next;
        }
    }
}

void store_allocation_state(void** state) {
    struct Node* curAlloc = allocator.allocatedBlocks;
    struct Node* curFree = allocator.freeBlocks;
    int index = 0;

    while (curAlloc != NULL) {
        state[index++] = curAlloc->block;
        curAlloc = curAlloc->next;
    }

    while (curFree != NULL) {
        state[index++] = curFree->block;
        curFree = curFree->next;
    }
}

int available_memory() {
    return allocator.available;
}

void print_statistics() {
    int allocatedSize = allocator.used;
    int allocatedChunks = listCountNodes(allocator.allocatedBlocks);
    int freeSize = allocator.available;
    int freeChunks = listCountNodes(allocator.freeBlocks);
    int smallestFreeChunkSize = allocator.size;
    int largestFreeChunkSize = 0;

    struct Node* cur = allocator.freeBlocks;

    if (allocator.freeBlocks == NULL) {
        smallestFreeChunkSize = 0;
    }

    while (cur != NULL) {
        if (cur->size < smallestFreeChunkSize) {
            smallestFreeChunkSize = cur->size;
        }
        if (cur->size > largestFreeChunkSize) {
            largestFreeChunkSize = cur->size;
        }
        cur = cur->next;
    }

    printf("Allocated size = %d\n", allocatedSize);
    printf("Allocated chunks = %d\n", allocatedChunks);
    printf("Free size = %d\n", freeSize);
    printf("Free chunks = %d\n", freeChunks);
    printf("Largest free chunk size = %d\n", largestFreeChunkSize);
    printf("Smallest free chunk size = %d\n", smallestFreeChunkSize);
}
