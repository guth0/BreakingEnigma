#include "stdio.h"
#include "stdlib.h"
#include "time.h"

void swap(char *c1, char *c2) {
  char temp = *c1;
  *c1 = *c2;
  *c2 = temp;
}

// Fisher-Yates shuffle
void shuffle(char *array, size_t n) {

  srand(time(NULL));

  for (int i = n - 1; i > 0; --i) {
    int rnd = rand() % (i + 1);

    swap(&array[i], &array[rnd]);
  }
}


int main(int argc, char *argv[]) {

  char *rotor = (char *)malloc(sizeof(char) * 26);

  for (int i = 0; i < 26; ++i) {
    rotor[i] = i + 'A';
  }

  shuffle(rotor, 26);

  printf("\"");
  for (int i = 0; i < 26; ++i) {
    printf("%c", rotor[i]);
  }
  printf("\"\n");

  free(rotor);

  return 0;
}
