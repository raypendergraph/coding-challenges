//
// Created by Ray Pendergraph on 2/27/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>

#define ALPHA .75f

#define UNDEFINED_ELEMENT 0
#define MAX_WORD_LENGTH 1024

void printStringArray(char *arr[], unsigned arrCount)
{
   printf("[");
   unsigned lastIdx = arrCount - 1;
   for (int i = 0; i < arrCount; i++)
   {
      printf("%s", arr[i]);
      if (i < lastIdx)
      {
         printf(" ");
      }
   }
   printf("]\n");
}

/******************************
 * WordSet
 ******************************/


struct Element
{
   unsigned long hash;
   // The null terminated string
#ifndef NDEBUG
   char *value;
#endif
   // The index of the next element in this bucket
   unsigned next;
//   // How many times this string was inserted
//   unsigned frequency;
//   // How many times this string has occurred in a comparison string.
//   unsigned occurences;
};

/**
 *
 */
struct WordSet
{
   // Word count of the original word set. This + 1 will also be the total element capacity.
   unsigned wordCount;
   // The word length (all are the same). Sequence length is wordCount * wordLength.
   unsigned wordLength;
   // Number of elements (unique words) in this set. wordCount - elementCount = duplicate count
   unsigned elementCount;
   // How many buckets in this set. This is calculated as ALPHA * wordCount
   unsigned bucketCount;
   // The elements or unique words (with metadata)
   struct Element *elements;
   // The hash buckets.
   unsigned *buckets;
};

unsigned long
djb2(const unsigned char *buffer, unsigned length)
{
   if (buffer == NULL)
   {
      return 0;
   }

   unsigned long hash = 5381;
   for (unsigned i = 0; i < length; i++)
   {
      // hash * 33 + c
      hash = ((hash << 5) + hash) + buffer[i];
   }
   return hash;
}

unsigned long
djb2Strings(unsigned char *strings[], size_t stringsCount, size_t stringLength)
{

   if (strings == NULL || stringsCount == 0)
   {
      return 0;
   }

   unsigned long hash = 5381;
   for (unsigned i = 0; i < stringsCount; i++)
   {
      for (unsigned j = 0; j < stringLength; j++)
      {
         // hash * 33 + c
         hash = ((hash << 5) + hash) + strings[i][j];
      }
   }
   return hash;
}

unsigned long
djb2NullTerminated(unsigned char *str)
{
   unsigned long hash = 5381;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c; // hash * 33 + c
   return hash;
}

char *
wordsToString(char *words[], unsigned wordsCount, unsigned wordLength)
{
   unsigned totalSize = wordsCount * wordLength;
   char *str = malloc(totalSize + 1);
   for (unsigned i = 0; i < wordsCount; i++)
   {
      memcpy(&str[i * wordLength], words[i], wordLength);
   }
   str[totalSize] = '\0';
   return str;
}

void
wordSetAddPermutation(struct WordSet *s, char *buffer[])
{
   unsigned long hash = djb2Strings((unsigned char **) buffer, s->wordCount, s->wordLength);
   unsigned
      bucketIdx = hash % s->bucketCount,
      idx = s->buckets[bucketIdx];

   // Bucket is empty
   if (idx == 0)
   {
      idx = ++s->elementCount;
      s->buckets[bucketIdx] = idx;
      goto setElement;
   }

   while (1)
   {
      if (s->elements[idx].hash == hash)
      {
         //assert(strncmp(s->elements[idx].value, wordsToString(buffer, s->wordCount, s->wordLength), s->wordLength) == 0);
         return;
      }

      // We are at the end of the chain, wire up the next one
      if (s->elements[idx].next == UNDEFINED_ELEMENT)
      {
         s->elementCount++;
         s->elements[idx].next = s->elementCount;
         idx = s->elementCount;
         goto setElement;
      }

      idx = s->elements[idx].next;
   }

   setElement:
   s->elements[idx] = (struct Element) {
      .hash=hash,
      .value=wordsToString(buffer, s->wordCount, s->wordLength)
   };
}


void
wordSetSetPermutations(struct WordSet *s, char *words[], unsigned wordsCount)
{
   unsigned *state = calloc(wordsCount, sizeof(unsigned));
   wordSetAddPermutation(s, words);

   for (unsigned level = 0; level < wordsCount;)
   {
      if (state[level] < level)
      {
         unsigned tmpIdx = level % 2 == 0 ? 0 : state[level];
         char *tmp = words[tmpIdx];
         words[tmpIdx] = words[level];
         words[level] = tmp;
         //printStringArray(words, s->wordCount);
         wordSetAddPermutation(s, words);
         state[level] += 1;
         level = 0;
         continue;
      }
      state[level] = 0;
      level += 1;
   }
   free(state);
}

bool
wordSetContains(struct WordSet *s, char *buffer, unsigned size)
{
   unsigned long hash = djb2((unsigned char *) buffer, size);
   for (unsigned i = s->buckets[hash % s->bucketCount]; i != UNDEFINED_ELEMENT; i = s->elements[i].next)
   {
      if (s->elements[i].hash == hash)
      {
         return true;
      }
   }
   return false;
}
unsigned
factorial(unsigned n)
{
   unsigned result = 1, i;

   for (i = 2; i <= n; i++)
   {
      result *= i;
   }

   return result;
}

struct WordSet *
wordSetNew(char *words[], unsigned wordsCount)
{
   unsigned elementCount = factorial(wordsCount) + 1;
   unsigned bucketCount = wordsCount < 2 ? 1 : (unsigned) ((float) wordsCount * ALPHA);
   struct WordSet *s = malloc(sizeof(struct WordSet));
   s->elementCount = 0;
   s->wordCount = wordsCount;
   s->wordLength = strnlen(words[0], MAX_WORD_LENGTH);
   s->bucketCount = bucketCount;
   s->elements = calloc(elementCount, sizeof (struct Element));
   s->buckets = calloc(bucketCount, sizeof (unsigned));
   wordSetSetPermutations(s, words, wordsCount);
   return s;
}

#ifndef NDEBUG
void
wordSetPrint(struct WordSet *s)
{
   printf("{");
   for (unsigned i = 0; i < s->bucketCount; i++)
   {
      for (unsigned j = s->buckets[i]; j != UNDEFINED_ELEMENT; j = s->elements[j].next)
      {
         printf("%s ", s->elements[j].value);
      }
   }
   printf("}\n");
}
#endif

/***************************
 * IntArray
 ***************************/
#define ARRAY_BLOCK_COUNT 128

struct IntArray
{
   size_t capacity;
   size_t count;
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
      a->data = realloc(a->data, a->capacity * 2);
   }
   a->data[a->count] = value;
   a->count++;
}

void printIntArray(unsigned arr[], unsigned arrCount)
{
   printf("[");
   unsigned lastIdx = arrCount - 1;
   for (int i = 0; i < arrCount; i++)
   {
      printf("%d", arr[i]);
      if (i < lastIdx)
      {
         printf(" ");
      }
   }
   printf("]\n");
}


/***************************
 * Context
 ***************************/
struct Context
{
   struct WordSet *words;
   struct IntArray *arr;
};

void printResults(int results[], int count)
{
   printf("[");
   for (unsigned i = 0; i < count; i++)
   {
      printf("%d ", results[i]);
   }
   printf("]\n");
}

int *
findSubstring(char *str, char **words, int wordsCount, int *returnSize)
{
   assert(words[0]);
   struct WordSet *s = wordSetNew(words, wordsCount);
   unsigned sampleLength = wordsCount * s->wordLength;
   //wordSetPrint(s);
   struct IntArray *a = intArrayNew();
   for (unsigned i = 0; i < sampleLength; i++)
   {
      if (str[i] == '\0')
      {
         *returnSize = 0;
         return NULL;
      }
   }
   for (unsigned i = 0; true; i++)
   {
      if (wordSetContains(s, &str[i], sampleLength))
      {
         intArrayAdd(a, (unsigned) i);
      }
      if (str[i + sampleLength] == '\0')
      {
        break;
      }

   }

   *returnSize = (int) a->count;
   return a->data;
}

int main()
{
   int *results = NULL;
   int returnCount;

   //Test case 1
//   results = findSubstring("barfoothefoobarman", (char *[]) {"foo", "bar"}, 2, &returnCount);
//   printResults(results, returnCount);
//
//   //Test case 2
//   results = findSubstring("wordgoodgoodgoodbestword", (char *[]) {"word","good","best","word"}, 4, &returnCount);
//   printResults(results, returnCount);
//
//   //Test case 2
//   results = findSubstring("barfoofoobarthefoobarman", (char *[]) {"bar","foo","the"}, 3, &returnCount);
//   printResults(results, returnCount);
//
//   //Test case 4
//   results = findSubstring("wordgoodgoodgoodbestword", (char *[]) {"word","good","best","good"}, 4, &returnCount);
//   printResults(results, returnCount);
//
//   //Test case 5
//   results = findSubstring("lingmindraboofooowingdingbarrwingmonkeypoundcake", (char *[]) {"fooo","barr","wing","ding","wing"}, 4, &returnCount);
//   printResults(results, returnCount);
//
//   //Test case 6
//   results = findSubstring("a", (char *[]) {"a"}, 1, &returnCount);
//   printResults(results, returnCount);

   //Test case 6
   results = findSubstring("pjzkrkevzztxductzzxmxsvwjkxpvukmfjywwetvfnujhweiybwvvsrfequzkhossmootkmyxgjgfordrpapjuunmqnxxdrqrfgkrsjqbszgiqlcfnrpjlcwdrvbumtotzylshdvccdmsqoadfrpsvnwpizlwszrtyclhgilklydbmfhuywotjmktnwrfvizvnmfvvqfiokkdprznnnjycttprkxpuykhmpchiksyucbmtabiqkisgbhxngmhezrrqvayfsxauampdpxtafniiwfvdufhtwajrbkxtjzqjnfocdhekumttuqwovfjrgulhekcpjszyynadxhnttgmnxkduqmmyhzfnjhducesctufqbumxbamalqudeibljgbspeotkgvddcwgxidaiqcvgwykhbysjzlzfbupkqunuqtraxrlptivshhbihtsigtpipguhbhctcvubnhqipncyxfjebdnjyetnlnvmuxhzsdahkrscewabejifmxombiamxvauuitoltyymsarqcuuoezcbqpdaprxmsrickwpgwpsoplhugbikbkotzrtqkscekkgwjycfnvwfgdzogjzjvpcvixnsqsxacfwndzvrwrycwxrcismdhqapoojegggkocyrdtkzmiekhxoppctytvphjynrhtcvxcobxbcjjivtfjiwmduhzjokkbctweqtigwfhzorjlkpuuliaipbtfldinyetoybvugevwvhhhweejogrghllsouipabfafcxnhukcbtmxzshoyyufjhzadhrelweszbfgwpkzlwxkogyogutscvuhcllphshivnoteztpxsaoaacgxyaztuixhunrowzljqfqrahosheukhahhbiaxqzfmmwcjxountkevsvpbzjnilwpoermxrtlfroqoclexxisrdhvfsindffslyekrzwzqkpeocilatftymodgztjgybtyheqgcpwogdcjlnlesefgvimwbxcbzvaibspdjnrpqtyeilkcspknyylbwndvkffmzuriilxagyerjptbgeqgebiaqnvdubrtxibhvakcyotkfonmseszhczapxdlauexehhaireihxsplgdgmxfvaevrbadbwjbdrkfbbjjkgcztkcbwagtcnrtqryuqixtzhaakjlurnumzyovawrcjiwabuwretmdamfkxrgqgcdgbrdbnugzecbgyxxdqmisaqcyjkqrntxqmdrczxbebemcblftxplafnyoxqimkhcykwamvdsxjezkpgdpvopddptdfbprjustquhlazkjfluxrzopqdstulybnqvyknrchbphcarknnhhovweaqawdyxsqsqahkepluypwrzjegqtdoxfgzdkydeoxvrfhxusrujnmjzqrrlxglcmkiykldbiasnhrjbjekystzilrwkzhontwmehrfsrzfaqrbbxncphbzuuxeteshyrveamjsfiaharkcqxefghgceeixkdgkuboupxnwhnfigpkwnqdvzlydpidcljmflbccarbiegsmweklwngvygbqpescpeichmfidgsjmkvkofvkuehsmkkbocgejoiqcnafvuokelwuqsgkyoekaroptuvekfvmtxtqshcwsztkrzwrpabqrrhnlerxjojemcxel",
                           (char *[]) {"dhvf","sind","ffsl","yekr","zwzq","kpeo","cila","tfty","modg","ztjg","ybty","heqg","cpwo","gdcj","lnle","sefg","vimw","bxcb"}, 18, &returnCount);
   printResults(results, returnCount);
}