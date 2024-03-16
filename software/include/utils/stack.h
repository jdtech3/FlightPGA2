#ifndef __UTILS_STACK_H
#define __UTILS_STACK_H

#include "stdlib.h"

typedef struct node {
    void* data;
    struct node* next;
} node_t;

typedef struct stack {
    node_t* head;
    int size;
} stack_t;

stack_t* stack_init();
void stack_push(stack_t*, void*);
void* stack_pop(stack_t*);
void stack_deinit(stack_t*);

#endif  // __UTILS_STACK_H
