#include "utils/stack.h"

stack_t* stack_init() {
    stack_t* stack = (stack_t*) malloc(sizeof(stack_t));
    stack->head = NULL;
    stack->size = 0;

    return stack;
}

void stack_push(stack_t* stack, void* data) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->data = data;
    node->next = stack->head;

    stack->head = node;
    stack->size++;
}

void* stack_pop(stack_t* stack) {
    node_t* node = stack->head;
    if (node == NULL) return NULL;

    stack->head = node->next;

    void* data = node->data;
    free(node);
    stack->size--;

    return data;
}

void stack_deinit(stack_t* stack) {
    while (stack_pop(stack) != NULL);
    free(stack);
}
