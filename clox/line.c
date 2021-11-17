#include "memory.h"
#include "line.h"

static void initDataArray(DataArray* array) {
  array->data = NULL;
  array->capacity = 0;
  array->count = 0;
}

static void writeDataArray(DataArray* array, int line) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->data = GROW_ARRAY(int, array->data,
                               oldCapacity, array->capacity);
  }

  array->data[array->count] = line;
  array->count++;
}

static void freeDataArray(DataArray* array) {
  FREE_ARRAY(int, array->data, array->capacity);
  initDataArray(array);
}

static void initIndexArray(IndexArray* array) {
  array->indexes = NULL;
  array->capacity = 0;
  array->count = 0;
}

static void writeIndexArray(IndexArray* array, int line) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->indexes = GROW_ARRAY(int, array->indexes,
                               oldCapacity, array->capacity);
  }

  array->indexes[array->count] = line;
  array->count++;
}

static void freeIndexArray(IndexArray* array) {
  FREE_ARRAY(int, array->indexes, array->capacity);
  initIndexArray(array);
}

void initLineArray(LineArray* array) {
  initDataArray(&array->dataArray);
  initIndexArray(&array->indexArray);
}

void writeLineArray(LineArray* array, int line) {
  if (array->dataArray.capacity > 0 
    && array->dataArray.data[array->dataArray.count - 1] == line) {
    array->indexArray.indexes[array->indexArray.count - 1]++;
  } else {
    writeDataArray(&array->dataArray, line);
    writeIndexArray(&array->indexArray, 1);
  }
}

void freeLineArray(LineArray* array) {
  freeDataArray(&array->dataArray);
  freeIndexArray(&array->indexArray);
}

int getLine(LineArray* array, int offset) {
  for (int index = 0, sum = 0; index < array->indexArray.count; index++) {
    sum += array->indexArray.indexes[index];
    if (sum > offset) {
      return array->dataArray.data[index];
    }
  }
  return 0;
}