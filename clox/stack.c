#include "memory.h"
#include "stack.h"

void initStack(Stack* stack) {
  stack->values = NULL;
  stack->capacity = 0;
  stack->count = 0;
}

void push(Stack* stack, Value value) {
  if (stack->capacity < stack->count + 1) {
    int oldCapacity = stack->capacity;
    stack->capacity = GROW_CAPACITY(oldCapacity);
    stack->values = GROW_ARRAY(Value, stack->values,
                               oldCapacity, stack->capacity);
  }

  stack->values[stack->count] = value;
  stack->count++;
}

Value pop(Stack* stack) {
  stack->count--;
  return stack->values[stack->count];
}

Value peek(Stack* stack, int distance) {
  return stack->values[stack->count - 1 - distance];
}

void freeStack(Stack* stack) {
  FREE_ARRAY(Value, stack->values, stack->capacity);
  initStack(stack);
}
