#ifndef LIST_H
#define LIST_H

struct Node {
    void* block;
    int size;
    struct Node *next;
};

// Allocate memory for a node and initialize it with the block and size. Return a pointer to the new node.
struct Node* listCreateNode(void* block, int size);

// Insert node at the head of the list.
void listInsertHead(struct Node **headRef, struct Node *node);

// Insert node at the tail of the list.
void listInsertTail(struct Node **headRef, struct Node *node);

// Count number of nodes in the list.
int listCountNodes(struct Node *head);

// Return the first node holding the block, or NULL if none found.
struct Node* listFindNode(struct Node *head, void* block);

// Delete node from the list and free memory allocated to it.
void listDeleteNode(struct Node **headRef, struct Node *node);

// Sort the list in ascending order based on the block field.
void listSort(struct Node **headRef);

// Free all nodes in the list and set the head to NULL.
void listFreeList(struct Node **headRef);

#endif
