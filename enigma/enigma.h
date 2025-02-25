#ifndef ENIGMA_H
#define ENIGMA_H

#include "stdlib.h"
#include "string.h"

#include "rotors.h"

struct config {

  // The arrays themselves are const but not the pointers
  const char *r1; // [26]
  const char *r2;
  const char *r3;
  const char *rfl; // [27]

  char *plugboard;

  int r1pos;
  int r2pos;
  int r3pos;

  int notch1;
  int notch2;

  //  int verbose;
};

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
/*
void spinRotors(char r1[27], char r2[27], char r3[27], int *r1count,
                int *r2count, int *r3count, const int notch1,
                const int notch2) {

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

// this is so fucking unreadable, I'm gonna continue with the one above
void spinRotors(struct config *cfg) {

  if (cfg->notch1 == cfg->r1pos) {
    cfg->r2pos = (cfg->r2pos + 1) % 26;
    rotate(cfg->r2, 1, 26);
    if (cfg->notch2 == cfg->r2pos) {
      cfg->r3pos = (cfg->r3pos + 1) % 26;
      rotate(cfg->r3, 1, 26);
    }
  }

  cfg->r1pos = (cfg->r1pos + 1) % 26;
  rotate(cfg->r1, 1, 26);
}
//

void fastRotate(const char **cypher, const int pos, const int count) {

  // calculate new offset
  int newPos = (pos + count) % 26;

  // apply new offset
  *cypher += newPos - pos;
}

void fastSpinRotors(const char *r1, const char *r2, const char *r3, int
*r1count, int *r2count, int *r3count, const int notch1, const int notch2) {

  if (notch1 == *r1count) {
    fastRotate(&r2, *r2count, 1);
    *r2count = (*r2count + 1) % 26;
    if (notch2 == *r2count) {
      fastRotate(&r3, *r3count, 1);
      *r3count = (*r3count + 1) % 26;
    }
  }

  fastRotate(&r1, *r1count, 1);
  *r1count = (*r1count + 1) % 26;
}

*/
void fasterSpinRotors(int *r1count, int *r2count, int *r3count,
                      const int notch1, const int notch2) {

  if (notch1 == *r1count) {
    *r2count = (*r2count + 1) % 26;
    if (notch2 == *r2count) {
      *r3count = (*r3count + 1) % 26;
    }
  }
  *r1count = (*r1count + 1) % 26;
}

char *Enigma(char *string, struct config *cfg) {

  char *output = (char *)malloc(strlen(string) + 1);

  for (int i = 0; i < strlen(string); i++) {

    char letter = string[i];
    char index = alphabetIndex(letter);

    if (index == -1) {
      // Skip all non-alphabetical characters
      output[i] = letter;
      continue;
    }

    index = cfg->plugboard[index];

    // these might be wrong as well but I don't think so
    index = cfg->r1[(index - cfg->r1pos + 26) % 26];
    index = cfg->r2[(index - cfg->r2pos + 26) % 26];
    index = cfg->r3[(index - cfg->r3pos + 26) % 26];
    // :)

    index = cfg->rfl[index];


    // NEED TO FIX THESE
    index = rotorIndex(cfg->r3, (index + cfg->r3pos) % 26);
    index = rotorIndex(cfg->r2, (index + cfg->r2pos) % 26);
    index = rotorIndex(cfg->r1, (index + cfg->r1pos) % 26);
    // :(

    index = cfg->plugboard[index];

    output[i] = index + 'A';

    // either this call is unreadable or the function itself is unreadable :(

    fasterSpinRotors(&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1,
                     cfg->notch2);

    //    spinRotors(cfg->r1, cfg->r2, cfg->r3, &cfg->r1pos, &cfg->r2pos,
    //    &cfg->r3pos,
    //               cfg->notch1, cfg->notch2);
  }

  output[strlen(string)] = '\0';

  return output;
}

#endif
