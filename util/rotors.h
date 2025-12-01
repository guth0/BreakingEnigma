#ifndef ROTORS_H
#define ROTORS_H

#include "stdio.h"

void spinRotors(int *r1count, int *r2count, int *r3count, const int notch1,
                const int notch2) {

  if (notch1 == *r1count) {
    if (notch2 == *r2count) {
      *r3count = (*r3count + 1) % 26;
    }
    *r2count = (*r2count + 1) % 26;
  }
  *r1count = (*r1count + 1) % 26;
}

// find the position of a character in the alphabet
char alphabetIndex(char target) {
  if (target >= 'A' && target <= 'Z') {
    return target - 'A';
  } else if (target >= 'a' && target <= 'z') {
    return target - 'a';
  } else {
    return -1;
  }
}

void printRotor(const char *arr) {
  for (int i = 0; i < 26; ++i) {
    printf("%c", arr[i] + 'A');
  }

  printf("\n");
}

#endif
