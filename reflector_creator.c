#include "stdio.h"
#include "stdlib.h"
#include "time.h"

void swap(char *arr, int index1, int index2) {
  char temp = arr[index1];
  arr[index1] = arr[index2];
  arr[index2] = temp;
}

void shuffle(char *array, size_t n) {

  srand(time(NULL));

  // we can do n - 1 here because the final character will always be shuffled
  for (int i = 0; i < n - 1; ++i) {
    // generate a random number
    int rnd = rand();
    int j = rnd % (n - (i * 2)) + 1;
    // j \in [1, numUntouched]

    // find the jth untouched number
    int k = i;
    int counter = 0;

    while (counter < j) {
      ++k; // k 'starts' at i + 1

      if (k >= n) {
        return;
      }

      if (array[k] == (k + 'A')) {
        ++counter;
      }
    }

    swap(array, i, k);
  }
}

char isValidReflector(char *arr) { 
  
  for (int i = 0; i< 26; ++i)
  {
    char c = arr[i];
    if (arr[c - 'A'] != i + 'A')
    {
      return 1;
    }
    // if the char at arr[i] is c, then the char at arr[c - 'A'] is i + 'A'
  }

  return 0; }

int main(int argc, char *argv[]) {

  char *reflector = (char *)malloc(sizeof(char) * 26);

  for (int i = 0; i < 26; ++i) {
    reflector[i] = i + 'A';
  }

  shuffle(reflector, 26);

  printf("\"");
  for (int i = 0; i < 26; ++i) {
    printf("%c", reflector[i]);
  }
  printf("\"\n");

  printf("This %s a valid reflector\n", (isValidReflector(reflector) == 0) ? "IS" : "IS NOT");

  free(reflector);

  return 0;
}
