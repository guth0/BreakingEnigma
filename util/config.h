#ifndef CONFIG_H
#define CONFIG_H

#include "stdio.h"
#include "stdlib.h"

struct Config {
  // The arrays themselves are const but not the pointers
  const char *r1; // [27]
  const char *r2;
  const char *r3;
  const char *rfl;

  char *plugboard;

  int r1pos;
  int r2pos;
  int r3pos;

  int notch1;
  int notch2;
};

struct Rotors {
  char *r[5];

  char *rfl[2];
};

int getRotorNum(const char *r, const struct Rotors *rotors) {

  for (int i = 0; i < 5; ++i) {
    if (r == rotors->r[i]) {
      // We don't have a 0 rotor
      return i + 1;
    }
  }

  // -1 for unfound rotor
  return -1;
}

int getReflectorNum(const char *r, const struct Rotors *rotors) {

  // just did an IF statement here b/c only two cases
  if (r == rotors->rfl[0]) {
    return 1;
  } else if (r == rotors->rfl[1]) {
    return 2;
  }

  // -1 for unfound reflector
  return -1;
}

void printConfig(const struct Config *cfg, const struct Rotors *rotors) {

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
