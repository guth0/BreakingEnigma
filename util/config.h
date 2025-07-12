#ifndef CONFIG_H
#define CONFIG_H

#include "stdio.h"
#include "stdlib.h"

struct Config {
  // The arrays themselves are const but not the pointers
  char r1[27];
  char r2[27];
  char r3[27];
  char rfl[27];

  // inverted rotors
  char inv_r1[27];
  char inv_r2[27];
  char inv_r3[27];

  char plugboard[27];

  int r1pos, r2pos, r3pos;

  int notch1, notch2;
};

struct FastConfig {
  // The arrays themselves are const but not the pointers
  char *r1; // [27]
  char *r2;
  char *r3;
  char *rfl;

  // inverted rotors
  char *inv_r1; // [27]
  char *inv_r2;
  char *inv_r3;

  char *plugboard;

  int r1pos, r2pos, r3pos;

  int notch1, notch2;
};

struct Rotors {
  char *r[5];

  char *rfl[2];
};

struct FastRotors {
  char *r[5];

  char *inv_r[5];

  char *rfl[2];
};

void invertRotor(const char *r, char *inverse) {
  inverse[26] = '\0';
  for (int i = 0; i < 26; ++i) {
    inverse[r[i]] = i;
  }
}

void invertConfig(struct Config *cfg) {
  invertRotor(cfg->r1, cfg->inv_r1);
  invertRotor(cfg->r2, cfg->inv_r2);
  invertRotor(cfg->r3, cfg->inv_r3);
}

int getRotorNum(const char *r, const struct FastRotors *rotors) {

  for (int i = 0; i < 5; ++i) {
    if (r == rotors->r[i]) {
      // We don't have a 0 rotor
      return i + 1;
    }
  }

  // -1 for unfound rotor
  return -1;
}

int getReflectorNum(const char *r, const struct FastRotors *rotors) {

  // just did an IF statement here b/c only two cases
  if (r == rotors->rfl[0]) {
    return 1;
  } else if (r == rotors->rfl[1]) {
    return 2;
  }

  // -1 for unfound reflector
  return -1;
}

void printConfig(const struct FastConfig *cfg,
                 const struct FastRotors *rotors) {

  // declare num to hold our
  int num;

  num = getRotorNum(cfg->r1, rotors);
  printf("Position 1: ROTOR #%d, p = %d\n", num, cfg->r1pos);

  num = getRotorNum(cfg->r2, rotors);
  printf("Position 2: ROTOR #%d, p = %d, n = %d\n", num, cfg->r2pos,
         cfg->notch1);

  num = getRotorNum(cfg->r3, rotors);
  printf("Position 3: ROTOR #%d, p = %d, n = %d\n", num, cfg->r3pos,
         cfg->notch2);

  num = getReflectorNum(cfg->rfl, rotors);
  printf("Position 4: REFLECTOR #%d\n", num);
}

#endif
