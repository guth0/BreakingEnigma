#ifndef ENIGMA_H
#define ENIGMA_H

#include "stdlib.h"
#include "string.h"

#include "rotors.h"

struct config {
  char r1[27];
  char r2[27];
  char r3[27];
  char rfl[27];

  char plugboard[27];

  int r1pos;
  int r2pos;
  int r3pos;

  int notch1;
  int notch2;
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

/* // this is so fucking unreadable, I'm gonna continue with the one above
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
*/

char *Enigma(char *string, struct config cfg) {

  char *output = (char *)malloc(strlen(string) + 1);

  for (int i = 0; i < strlen(string); i++) {

    char letter = string[i];
    char index = alphabetIndex(letter);

    if (index == -1) {
      // Skip all non-alphabetical characters
      output[i] = letter;
      continue;
    }

    index = cfg.plugboard[index];

    index = cfg.r1[index];
    index = cfg.r2[index];
    index = cfg.r3[index];

    index = cfg.rfl[index];

    index = rotorIndex(cfg.r3, index);
    index = rotorIndex(cfg.r2, index);
    index = rotorIndex(cfg.r1, index);

    index = cfg.plugboard[index];

    output[i] = index + 'A';


    // either this call is unreadable or the function itself is unreadable :(
    spinRotors(cfg.r1, cfg.r2, cfg.r3, &cfg.r1pos, &cfg.r2pos, &cfg.r3pos, cfg.notch1, cfg.notch2);
  }

  output[strlen(string)] = '\0';

  return output;
}

#endif
