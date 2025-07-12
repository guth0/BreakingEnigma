#ifndef ENIGMA_H
#define ENIGMA_H

#include "../util/config.h"
#include "../util/rotors.h"

#include "stdlib.h"
#include "string.h"

char *Enigma(char *string, struct Config *cfg) {

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

    index = cfg->r1[(index - cfg->r1pos + 26) % 26];
    index = cfg->r2[(index - cfg->r2pos + 26) % 26];
    index = cfg->r3[(index - cfg->r3pos + 26) % 26];

    index = cfg->rfl[index];

    index = (cfg->inv_r3[index] + cfg->r3pos) % 26;
    index = (cfg->inv_r2[index] + cfg->r2pos) % 26;
    index = (cfg->inv_r1[index] + cfg->r1pos) % 26;

    index = cfg->plugboard[index];

    output[i] = index + 'A';

    spinRotors(&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1, cfg->notch2);
  }

  output[strlen(string)] = '\0';

  return output;
}

#endif
