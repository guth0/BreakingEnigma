#include "stdio.h"
#include "stdlib.h"
#include "time.h"


void swap(char *c1, char *c2) {
  char temp = *c1;
  *c1 = *c2;
  *c2 = temp;
}

void shuffle(char *array, size_t n) {

  srand(time(NULL));

  // we can do n - 1 here because the final character will always be shuffled

  if (n % 2 != 0) {
    fprintf(stderr, "String length must be even");
    return;
  }

  for (int i = 0; i < n / 2; ++i) {
    int c1, c2;

    int rnd = rand();
    c1 = rnd % (n - (i * 2));
    rnd = rand();
    c2 = rnd % (n - (i * 2) - 1);

    char *p1, *p2;

    int j = 0;
    while (c1 >= 0 || c2 >= 0) {

      // if letter is in original spot
      if (array[j] == j + 'A') {

        if (c1 == 0) {

	  c2++;
          p1 = &array[j];

        } else if (c2 == 0) {

          p2 = &array[j];

        }

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
