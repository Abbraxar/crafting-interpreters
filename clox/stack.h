#ifndef clox_stack_h
#define clox_stack_h

#include "common.h"
#include "value.h"

typedef struct {
  int capacity;
  int count;
  Value* values;
} Stack;

void initStack(Stack* stack);
void push(Stack* stack, Value value);
Value pop(Stack* stack);
Value peek(Stack* stack, int distance);
void freeStack(Stack* stack);

#endif