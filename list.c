#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct Node* listCreateNode(void* block, int size) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->block = block;
    node->size = size;
    node->next = NULL;
    return node;
}

void listInsertHead(struct Node **headRef, struct Node *node) {
    node->next = *headRef;
    *headRef = node;
}

void listInsertTail(struct Node **headRef, struct Node *node) {
    if (*headRef == NULL) {
        listInsertHead(headRef, node);
        return;
    }
    struct Node* cur = *headRef;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    node->next = NULL;
    cur->next = node;
}

int listCountNodes(struct Node *head) {
    int count = 0;
    struct Node* cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

struct Node* listFindNode(struct Node *head, void* block) {
    struct Node* cur = head;
    while (cur != NULL) {
        if (cur->block == block) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void listDeleteNode(struct Node **headRef, struct Node *node) {
    if (*headRef == node) {
        *headRef = node->next;
        free(node);
        return;
    }
    struct Node* cur = *headRef;
    while (cur->next != NULL && cur->next != node) {
        cur = cur->next;
    }
    if (cur->next == node) {
        cur->next = node->next;
        free(node);
    }
}

void listSort(struct Node **headRef) {
    struct Node* sorted = NULL;
    struct Node* cur = *headRef;
    while (cur != NULL) {
        struct Node* next = cur->next;
        if (sorted == NULL || cur->block < sorted->block) {
            cur->next = sorted;
            sorted = cur;
        } else {
            struct Node* s = sorted;
            while (s->next != NULL && s->next->block < cur->block) {
                s = s->next;
            }
            cur->next = s->next;
            s->next = cur;
        }
        cur = next;
    }
    *headRef = sorted;
}

void listFreeList(struct Node **headRef) {
    struct Node* cur = *headRef;
    struct Node* next = NULL;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    *headRef = NULL;
}
