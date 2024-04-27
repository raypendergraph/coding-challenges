#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>

struct ListNode {
   int val;
   struct ListNode *next;
};

struct PriorityQueue
{
   unsigned count;
   unsigned capacity;
   struct ListNode *nodes[];
};

static void
heapifyDown(struct PriorityQueue *q, unsigned idx, struct ListNode *carryNode)
{
   // assert n > 0;
   unsigned half = q->count / 2;
   while (idx < half) // loop while a non-leaf
   {
      unsigned
         leftIdx = 2 * idx + 1,
         rightIdx = leftIdx + 1,
         nextIdx = leftIdx;

      struct ListNode
         *leftNode = q->nodes[leftIdx],
         *rightNode = q->nodes[rightIdx],
         *nextNode = leftNode;

      if (rightIdx < q->count && leftNode->val > rightNode->val)
      {
         nextIdx = rightIdx;
         nextNode = rightNode;
      }

      if (carryNode->val <= nextNode->val)
      {
         break;
      }

      q->nodes[idx] = nextNode;
      idx = nextIdx;
   }

   q->nodes[idx] = carryNode;
}

void
heapify(struct PriorityQueue *q)
{

   for (int i = ((int)q->count / 2) - 1; i >= 0; i--)
   {
      heapifyDown(q, i, q->nodes[i]);
   }
}

static void
heapifyUp(struct PriorityQueue *q, unsigned idx, struct ListNode *carryNode)
{
   while (idx > 0)
   {
      unsigned parentIdx = idx / 2;
      struct ListNode *parentValue = q->nodes[parentIdx];
      if (parentValue->val <= carryNode->val)
      {
         break;
      }
      q->nodes[idx] = parentValue;
      idx = parentIdx;
   }
   q->nodes[idx] = carryNode;
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
      q->nodes[idx] = NULL;
      return true;
   }

   struct ListNode *lastNode = q->nodes[lastIdx];
   q->nodes[lastIdx] = NULL;
   heapifyDown(q, idx, lastNode);
   if (q->nodes[idx]->val == lastNode->val)
   {
      heapifyUp(q, idx, lastNode);
   }

   return true;
}
///////////////////////////////////////////


struct PriorityQueue *
priorityQueueNew(size_t size)
{
   return malloc(sizeof (struct PriorityQueue) + size * sizeof (struct ListNode));
}

struct ListNode *
reduce(struct PriorityQueue *q)
{
   if (q->count == 0)
   {
      return NULL;
   }
   struct ListNode
      *head = q->nodes[0],
      *current = NULL;
   do
   {
      // list advance
      current = q->nodes[0];
      if (current->next == NULL)
      {
         priorityQueueDelete(q, 0);
      }
      else
      {
         q->nodes[0] = q->nodes[0]->next;
         heapifyDown(q, 0, q->nodes[0]);
      }

      current->next = q->nodes[0];
   }
   while(q->count > 0);
   return head;
}

struct ListNode*
mergeKLists(struct ListNode** lists, int listsSize) {
   struct PriorityQueue *q = priorityQueueNew(listsSize);
   q->count = 0;
   for (size_t i = 0, trueIdx = 0; i < listsSize; i++)
   {
      if (lists[i] == NULL)
      {
         continue;
      }
      q->nodes[trueIdx++] = lists[i];
      q->count = trueIdx;
   }
   q->capacity = q->count;
   heapify(q);
   return reduce(q);
}

struct ListNode*
createLinkedList(const int *values, size_t size)
{
   struct ListNode *nodes = malloc(size * sizeof(*nodes));
   nodes[0].val = values[0];
   if (size == 1)
   {
      return nodes;
   }

   for (size_t i = 1; i < size; i++)
   {
      nodes[i - 1].next = &nodes[i];
      nodes[i].val = values[i];
   }
   return nodes;
}

void
printList(struct ListNode* list)
{
   printf("[");
   if (list == NULL)
   {
      goto close;
   }
   while((list = list->next) != NULL)
   {
      printf(", %d", list->val);
   }

   close:
   printf("]\n");
}

int main()
{
   //Testcase #1
   //[[1,4,5],[1,3,4],[2,6]]
   struct ListNode *lists1[]= {
      createLinkedList((int[]){1, 4, 5}, 3),
      createLinkedList((int[]){1, 3, 4}, 3),
      createLinkedList((int[]){2, 6}, 2),
   };

   struct ListNode *list1 = mergeKLists(lists1, 3);
   printList(list1);

   //Testcase #2
   //[]
   struct ListNode *lists2[]= {};

   struct ListNode *list2 = mergeKLists(lists1, 0);
   printList(list2);

   //Testcase #2
   //[[]]
   struct ListNode *lists3[]= { NULL };

   struct ListNode *list3 = mergeKLists(lists3, 1);
   printList(list3);

   //Testcase #4
   //[[],[]]
   struct ListNode *lists4[]= { NULL, NULL };

   struct ListNode *list4 = mergeKLists(lists4, 2);
   printList(list4);
}