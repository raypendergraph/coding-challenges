#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <libc.h>

struct PriorityQueue
{
   unsigned count;
   unsigned capacity;
   int elements[];
};

static void
heapifyUp(struct PriorityQueue *q, unsigned idx, int x)
{
   while (idx > 0)
   {
      unsigned parentIdx = idx / 2;
      int parentValue = q->elements[parentIdx];
      if (parentValue <= x)
      {
         break;
      }
      q->elements[idx] = parentValue;
      idx = parentIdx;
   }
   q->elements[idx] = x;
}


static void
printSpaces(unsigned count)
{
   for (unsigned i = 0; i < count; i++)
   {
      printf(" ");
   }
}

unsigned
priorityQueueGetHeight(struct PriorityQueue *q)
{
   return (unsigned) floor(log2(q->capacity));
}

void
priorityQueuePrint(struct PriorityQueue *q)
{
   unsigned height = priorityQueueGetHeight(q);
   unsigned index = 0;

   for (unsigned i = 0; i < height; i++)
   {
      unsigned elementsAtLevel = (unsigned) pow(2, i);
      unsigned spaces = (unsigned) pow(2, (height - i)) - 1;
      printSpaces(spaces / 2);

      for (unsigned j = 0; j < elementsAtLevel && index < q->capacity; j++, index++)
      {
         printf("%d", q->elements[index]);
         if (j < elementsAtLevel - 1)
         {
            printSpaces(spaces);
         }
      }
      printf("\n");
   }
}


static void
heapifyDown(struct PriorityQueue *q, unsigned idx, int x)
{
   // assert n > 0;
   unsigned half = q->count / 2;
   while (idx < half) // loop while a non-leaf
   {
      unsigned
         leftIdx = 2 * idx + 1,
         rightIdx = leftIdx + 1,
         nextIdx = leftIdx;

      int
         leftValue = q->elements[leftIdx],
         rightValue = q->elements[rightIdx],
         nextValue = leftValue;

      if (rightIdx < q->count && leftValue > rightValue)
      {
         nextIdx = rightIdx;
         nextValue = rightValue;
      }

      if (x <= nextValue)
      {
         break;
      }

     q->elements[idx] = nextValue;
      idx = nextIdx;
   }

   q->elements[idx] = x;
}

void
heapify(struct PriorityQueue *q)
{

   for (int i = ((int)q->count / 2) - 1; i >= 0; i--)
   {
      heapifyDown(q, i, q->elements[i]);
   }
}

bool
priorityQueueAdd(struct PriorityQueue *q, int weight)
{
   if (q->count == q->capacity)
   {
      return false;
   }

   heapifyUp(q, q->count, weight);
   q->count += 1;
   return true;
}

bool
priorityQueueDelete(struct PriorityQueue *q, unsigned idx)
{
   if (q->count == 0)
   {
      return false;
   }
   unsigned lastIdx = --q->count;
   if (idx == lastIdx) // if we have removed the last element we are done
   {
      q->elements[idx] = 0;
      return true;
   }

   int lastValue = q->elements[lastIdx];
   q->elements[lastIdx] = 0;
   heapifyDown(q, idx, lastValue);
   if (q->elements[idx] == lastValue)
   {
      heapifyUp(q, idx, lastValue);
   }

   return true;
}

struct PriorityQueue *
priorityQueueNew(size_t size)
{
   return malloc(sizeof (struct PriorityQueue) + size * sizeof (int));
}

struct PriorityQueue *
priorityQueueNewFromElements(size_t count, int *elements)
{
   struct PriorityQueue *q = priorityQueueNew(count);
   memcpy(q->elements, elements, count * sizeof (int));
   q->capacity = count;
   q->count = count;
   heapify(q);
   return q;
}

static bool
randomIntArray(unsigned seed, size_t count, int **elements) {
   *elements = (int*)malloc(count * sizeof(int));
   if (*elements == NULL) {
      return false;
   }

   srand(seed);
   for (size_t i = 0; i < count; i++)
   {
      // rand() % 2000 gives a range [0, 1999]. Subtract 1000 to shift to [-999, 999]
      (*elements)[i] = (rand() % 2000) - 1000;
   }
   return true;
}

bool
priorityQueueValidate(struct PriorityQueue *q)
{
   unsigned firstLeafNodeIdx = q->count / 2 - 1;
   for (unsigned i = 0;i < firstLeafNodeIdx; i++) {
      unsigned leftIdx = 2 * i + 1,
         rightIdx = leftIdx+1;

      if (leftIdx < q->count && q->elements[i] > q->elements[leftIdx]) {
         printf("element %d (%d) has a left child that is less than it: %d.\n", i, q->elements[i], q->elements[leftIdx]);
         return false;
      }

      if (rightIdx < q->count && q->elements[i] > q->elements[rightIdx]) {
         printf("element %d (%d) has a right child that is less than it: %d.\n", i, q->elements[i], q->elements[rightIdx]);
         return false;
      }
   }
   return true;
}

int main()
{
   int fd = open("/dev/urandom", O_RDONLY);
   const unsigned iterations = 100000, heapSize = 8;
   printf("Testing with %d iterations\n", iterations);
   unsigned seed;
   for (unsigned i = 0 ; i < iterations; i++)
   {
      int *r;
      read(fd, &seed, sizeof(seed));
      srand(seed);
      randomIntArray(seed, heapSize,&r);
      struct PriorityQueue *q = priorityQueueNewFromElements(heapSize, r);
      if (!priorityQueueValidate(q))
      {
         priorityQueuePrint(q);
         break;
      }

      free(r);
   }
   close(fd);
}
