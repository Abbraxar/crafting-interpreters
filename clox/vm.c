#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "vm.h"

VM vm; 

void initVM() {
  initStack(&vm.stack);
  vm.objects = NULL;
}

void freeVM() {
  freeObjects();
}

static bool isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() {
  ObjString* b = AS_STRING(pop(&vm.stack));
  ObjString* a = AS_STRING(pop(&vm.stack));

  int length = a->length + b->length;
  char* chars = ALLOCATE(char, length + 1);
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';

  ObjString* result = takeString(chars, length);
  push(&vm.stack, OBJ_VAL(result));
}

static void runtimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code - 1;
  int line = getLine(&vm.chunk->lines, instruction);
  fprintf(stderr, "[line %d] in script\n", line);
  initStack(&vm.stack);
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(&vm.stack, 0)) || !IS_NUMBER(peek(&vm.stack, 1))) { \
        runtimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop(&vm.stack)); \
      double a = AS_NUMBER(pop(&vm.stack)); \
      push(&vm.stack, valueType(a op b)); \
    } while (false)
#define IS_TRUTHY(value) (!IS_NIL(value) && ((IS_NUMBER(value) && AS_NUMBER(value)) || (IS_BOOL(value) && AS_BOOL(value))))

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
      case OP_NIL:      push(&vm.stack, NIL_VAL); break;
      case OP_TRUE:     push(&vm.stack, BOOL_VAL(true)); break;
      case OP_FALSE:    push(&vm.stack, BOOL_VAL(false)); break;
      case OP_EQUAL: {
        Value b = pop(&vm.stack);
        Value a = pop(&vm.stack);
        push(&vm.stack, BOOL_VAL(valuesEqual(a, b)));
        break;
      }
      case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
      case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;
      case OP_ADD: {
        if (IS_STRING(peek(&vm.stack, 0)) && IS_STRING(peek(&vm.stack, 1))) {
          concatenate();
        } else if (IS_NUMBER(peek(&vm.stack, 0)) && IS_NUMBER(peek(&vm.stack, 1))) {
          double b = AS_NUMBER(pop(&vm.stack));
          double a = AS_NUMBER(pop(&vm.stack));
          push(&vm.stack, NUMBER_VAL(a + b));
        } else {
          runtimeError(
              "Operands must be two numbers or two strings.");
          return INTERPRET_RUNTIME_ERROR;
        }
        break;
      }
      case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
      case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
      case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
      case OP_NOT:
        push(&vm.stack, BOOL_VAL(isFalsey(pop(&vm.stack))));
        break;
      case OP_NEGATE:
        if (!IS_NUMBER(peek(&vm.stack, 0))) {
          runtimeError("Operand must be a number.");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(&vm.stack, NUMBER_VAL(-AS_NUMBER(pop(&vm.stack))));
        break;
      case OP_TERNARY: {
        // Value right = pop(&vm.stack);
        // Value middle = pop(&vm.stack);
        // Value left = pop(&vm.stack);
        // if (!isFalsey(left)) {
        //   push(&vm.stack, middle);
        // } else {
        //   push(&vm.stack, right);
        // }
        // break;
      }
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

InterpretResult interpret(const char* source) {
  Chunk chunk;
  initChunk(&chunk);

  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
}