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
  char *r[5];

  char *rfl[2];
};

char *getRotorName(const char *r, const struct Rotors *rotors) {

  // 8 here b/c "ROTOR X\0" 
  char returnString[8];

  for (int i = 0; i < 5; ++i){
     if (r == rotors->r[i])
     {
       // 6 is always where the number goes
       // add i to '1' to get the correct rotor number
       returnString[6] = '1' + i;
       
     }

  }

  return "NO MATCHING ROTOR";
}

char *getReflectorName(const char *r, const struct Rotors *rotors) {

  // just did an IF statement here b/c only two cases
  if (r == rotors->rfl[1]) {
    return "Reflector 1";
  } else if (r == rotors->rfl[2]) {
    return "Reflector 2";
  }

  return "NO MATCHING REFLECTOR";
}

void printConfig(const struct Config *cfg, const struct Rotors *rotors) {
  printf("Position 1: %s, p = %d\n", getRotorName(cfg->r1, rotors), cfg->r1pos);
  printf("Position 2: %s, p = %d, n = %d\n", getRotorName(cfg->r2, rotors),
         cfg->r2pos, cfg->notch1);
  printf("Position 3: %s, p = %d, n = %d\n", getRotorName(cfg->r3, rotors),
         cfg->r3pos, cfg->notch2);

  printf("Position 4: %s\n", getReflectorName(cfg->rfl, rotors));
}

#endif
