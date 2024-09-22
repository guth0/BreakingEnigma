#include "stdio.h"
#include "stdlib.h"
#include "time.h"

void swap(char *c1, char *c2) {
  char temp = *c1;
  *c1 = *c2;
  *c2 = temp;
}

void shuffle(char *array, size_t n) {

  if (n % 2 != 0) {
    fprintf(stderr, "String length must be even");
    return;
  }

  srand(time(NULL));

  for (int i = 0; i < n / 2; ++i) {
    int c1, c2;

    // make c1, c2 random numbers
    // c1 must be between 0 and the number of unmoved letters
    // c2 must be between 0 and 1 - number of unmoved letters
    int rnd = rand();
    c1 = rnd % (n - (i * 2));
    rnd = rand();
    c2 = rnd % (n - (i * 2) - 1);

    char *p1, *p2;

    int j = 0;
    while (c1 >= 0 || c2 >= 0) {

      // if letter is in original spot
      if (array[j] == j + 'A') {

        // if c1 == 0, then we have found the c1st unmoved letter
        if (c1 == 0) {

          // increment c2 here because the c1st unmoved letter
	  // does not count for c2
          c2++;
	  // I know this ^^ is bad
	  // but it makes it more readable than the alterantive

          p1 = &array[j];

	  // if c2 == 0, then we have found the c2nd* unmoved letter
	  // * -> not including the c1st
        } else if (c2 == 0) {

          p2 = &array[j];
        }

	// decrement the counters
        c1--;
        c2--;
      }

      j++;
    }

    swap(p1, p2);
  }
}

char isValidReflector(char *arr) {

  for (int i = 0; i < 26; ++i) {
    char c = arr[i];
    if (arr[c - 'A'] != i + 'A') {
      printf("arr[%d] = %c, arr['%c' - 'A'] != %d + 'A'", i, c, c, i);
      return 1;
    }
    // if the char at arr[i] is c, then the char at arr[c - 'A'] is i + 'A'
  }

  return 0;
}

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

  if (argc == 2 && argv[1][1] == 'v') {
    printf("\"ABCDEFGHIJKLMNOPQRSTUVWXYZ\"\n");
    printf("This %s a valid reflector\n",
           (isValidReflector(reflector) == 0) ? "IS" : "IS NOT");
  }

  free(reflector);

  return 0;
}
