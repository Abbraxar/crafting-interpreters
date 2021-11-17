#ifndef clox_line_h
#define clox_line_h

#include "common.h"

typedef struct {
  int capacity;
  int count;
  int* data;
} DataArray;

typedef struct {
  int capacity;
  int count;
  int* indexes;
} IndexArray;

typedef struct {
  DataArray dataArray;
  IndexArray indexArray;
} LineArray;

void initLineArray(LineArray* array);
void writeLineArray(LineArray* array, int line);
void freeLineArray(LineArray* array);
int getLine(LineArray* array, int offset);

#endif