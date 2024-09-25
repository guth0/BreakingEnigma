#ifndef ENIGMA_H
#define ENIGMA_H

#include "stdlib.h"
#include "string.h"

#include "rotors.h"

char alphabetIndex(char target) {
  if (target >= 'A' && target <= 'Z') {
    return target - 'A';
  } else if (target >= 'a' && target <= 'z') {
    return target - 'a';
  } else {
    return -1;
  }
}

char rotorIndex(const char *rotor, char target) {
  for (int i = 0; i < 26; ++i) {
    if (target == rotor[i]) {
      return i;
    }
  }

  return -1;
}

void spinRotors(char *r1, char *r2, char *r3, int *r1count, int *r2count,
                int *r3count, const int notch1, const int notch2) {

  if (notch1 == *r1count) {
    *r2count = (*r2count + 1) % 26;
    rotate(r2, 1, 26);
    if (notch2 == *r2count) {
      *r3count = (*r3count + 1) % 26;
      rotate(r3, 1, 26);
    }
  }

  *r1count = (*r1count + 1) % 26;
  rotate(r1, 1, 26);
}

char *Enigma(char *string, char r1[26], char r2[26], char r3[26], char rfl[26], int r1pos,
             int r2pos, int r3pos, const char plugboard[26], const int notch1,
             const int notch2) {

  char *output = (char *)malloc(strlen(string) + 1);

  for (int i = 0; i < strlen(string); i++) {

    char letter = string[i];
    char index = alphabetIndex(letter);
    
    if (index == -1)
    {
      // Skip all non-alphabetical characters
      output[i] = letter;
      continue;
    }

    index = plugboard[index];

    index = r1[index];
    index = r2[index];
    index = r3[index];

    index = rfl[index];

    index = rotorIndex(r3, index);
    index = rotorIndex(r2, index);
    index = rotorIndex(r1, index);

    index = plugboard[index];

    output[i] = index + 'A';

    spinRotors(r1, r2, r3, &r1pos, &r2pos, &r3pos, notch1, notch2);
  }

  output[strlen(string)] = '\0';

  return output;
}

#endif
