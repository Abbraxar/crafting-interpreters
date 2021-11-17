#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

VM vm; 

void initVM() {
  initStack(&vm.stack);
}

void freeVM() {
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
      double b = pop(&vm.stack); \
      double a = pop(&vm.stack); \
      push(&vm.stack, a op b); \
    } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
  printf("          ");
  for (int i = 0; i < vm.stack.count; i++) {
    printf("[ ");
    printValue(vm.stack.values[i]);
    printf(" ]");
  }
  printf("\n");
  disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(&vm.stack, constant);
        break;
      }
      case OP_CONSTANT_LONG: {
        uint8_t b1 = READ_BYTE(), b2 = READ_BYTE(), b3 = READ_BYTE();
        Value constant = vm.chunk->constants.values[(b1 << 16) + (b2 << 8) + b3];
        push(&vm.stack, constant);
        break;
      }
      case OP_ADD:      BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;
      case OP_NEGATE: push(&vm.stack, -pop(&vm.stack)); break;
      case OP_RETURN: {
        printValue(pop(&vm.stack));
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}