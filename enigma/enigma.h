#ifndef ENIGMA_H
#define ENIGMA_H

#include "../util/config.h"
#include "../util/rotors.h"

#include "stdlib.h"
#include "string.h"

char *Enigma(char *string, struct Config *cfg) {

  // init string for return
  char *output = (char *)malloc(strlen(string) + 1);

  // encrypt
  for (int i = 0; i < strlen(string); i++) {

    char letter = string[i];
    char index = alphabetIndex(letter);

    if (index == -1) {
      // Skip all non-alphabetical characters
      output[i] = letter;
      continue;
    }

    // printf("p1: %d, p2: %d, p3: %d\n", cfg->r1pos, cfg->r2pos, cfg->r3pos);

    // plugboard forwards
    index = cfg->plugboard[index];

    // rotors forward
    index = cfg->r1[(index - cfg->r1pos + 26) % 26];
    index = cfg->r2[(index - cfg->r2pos + 26) % 26];
    index = cfg->r3[(index - cfg->r3pos + 26) % 26];

    // reflector
    index = cfg->rfl[index];

    // rotors backward
    index = (rotorIndex(cfg->r3, index) + cfg->r3pos) % 26;
    index = (rotorIndex(cfg->r2, index) + cfg->r2pos) % 26;
    index = (rotorIndex(cfg->r1, index) + cfg->r1pos) % 26;

    // plugboard backwards 
    // (It is symetric so it is the same as forwards)
    index = cfg->plugboard[index];

    output[i] = index + 'A';

    spinRotors(&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1, cfg->notch2);
  }

  output[strlen(string)] = '\0';

  return output;
}

#endif
