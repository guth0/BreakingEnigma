#ifndef CONFIG_H
#define CONFIG_H

#include "stdio.h"

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
  char *r1;
  char *r2;
  char *r3;
  char *r4;
  char *r5;

  char *rfl1;
  char *rfl2;
};

char *getRotorName(const char *r, const struct Rotors *rotors) {
  if (r == rotors->r1) {
    return "Rotor 1";
  } else if (r == rotors->r2) {
    return "Rotor 2";
  } else if (r == rotors->r3) {
    return "Rotor 3";
  } else if (r == rotors->r4) {
    return "Rotor 4";
  } else if (r == rotors->r5) {
    return "Rotor 5";
  }

  return "ERROR";
}

char *getReflectorName(const char *r, const struct Rotors *rotors) {

  if (r == rotors->rfl1) {
    return "Reflector 1";
  } else if (r == rotors->rfl2) {
    return "Reflector 2";
  }

  return "ERROR";
}

void printConfig(const struct Config *cfg, const struct Rotors *rotors) {
  printf("Position 1: %s, p = %d\n", getRotorName(cfg->r1, rotors), cfg->r1pos);
  printf("Position 2: %s, p = %d, n = %d\n", getRotorName(cfg->r2, rotors),
         cfg->r2pos, cfg->notch1);
  printf("Position 3: %s, p = %d, n = %d\n", getRotorName(cfg->r3, rotors),
         cfg->r3pos, cfg->notch2);

  printf("Position 4: %s\n", getReflectorName(cfg->r3, rotors));
}

#endif
