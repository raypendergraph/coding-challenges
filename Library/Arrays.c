//
// Created by Ray Pendergraph on 3/4/24.
//
#include <stdlib.h>
#include <assert.h>

#define ARRAY_BLOCK_COUNT 128
struct IntArray
{
   size_t
      capacity,
      count,
      head,
      tail;
   int *data;
};

struct IntArray *
intArrayNew()
{
   struct IntArray *a = malloc(sizeof(*a));
   int *data = malloc(sizeof(int) * ARRAY_BLOCK_COUNT);
   a->count = 0;
   a->capacity = ARRAY_BLOCK_COUNT;
   a->data = data;
   return a;
}

void
intArrayAdd(struct IntArray *a, int value)
{
   if (a->count == a->capacity)
   {

      a->capacity *= 2;
      a->data = reallocf(a->data, a->capacity);
   }
   a->data[a->count] = value;
   a->count++;
}
