#include "../util/config.h"
#include "../util/rotors.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Plan (Crib Centered):
// 	1. Crib HAS to exist at the very start of the message
// 	2. Crib can be anywhere in the message
// 		a. This is probably harder than it sounds
//
// 	3. Impliment plugboard
// 	3. Multiple Cribs (Idk if this is usefull or not)
//
//

char ROTOR_1[27] = "EKMFGHDQVZXRUTIABJSYCLWPON";
char ROTOR_2[27] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
char ROTOR_3[27] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
char ROTOR_4[27] = "KCPXSYOMZRTLEVFWUNAIQGHBDJ";
char ROTOR_5[27] = "LZIJTBYNREOAPUVDWHXCKQMGFS";

char REFLECTOR_1[27] = "COAHIJRDEFQZWYBUKGXVPTMSNL";
char REFLECTOR_2[27] = "WKPZGHEFVQBXRYUCJMTSOIALND";

char ALPHABET[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int shortEnigma(char *crib, char *cypher, struct FastConfig *cfg) {

  for (int i = 0; i < strlen(crib); i++) {

    char letter = crib[i];
    char index = alphabetIndex(letter);

    // if non-alphabetical:
    if (index == -1) {

      if (index != cypher[i]) {
        return 0;
      }

      continue;
    }

    // Send through Enigma

    // index = cfg->plugboard[index];

    index = cfg->r1[(index - cfg->r1pos + 26) % 26];
    index = cfg->r2[(index - cfg->r2pos + 26) % 26];
    index = cfg->r3[(index - cfg->r3pos + 26) % 26];

    index = cfg->rfl[index]; // reflector

    index = (cfg->inv_r3[index] + cfg->r3pos) % 26;
    index = (cfg->inv_r2[index] + cfg->r2pos) % 26;
    index = (cfg->inv_r1[index] + cfg->r1pos) % 26;

    // index = cfg->plugboard[index];

    // check for match
    if (cypher[i] != index + 'A') {
      return 0;
    }

    spinRotors(&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1, cfg->notch2);
  }

  return 1;
}

// since the Config is passed by pointer, the function can just return 1 if a
// correct position is found and 0 if not
// optimized enigma function
// 	no plugboard (for now)
// 	stops at the first incorrect letter
// 	returns 1 if encyrpted crib = cypher, 0 if not
//
int testRotors(char *crib, char *cypher, struct FastConfig *cfg) {

  // go over every rotation and notch combo in a loop here

  // if ((notch1 - r1pos + 26) % 26 < cribLen), notch1 will hit
  //
  //      (notch1 - r1pos + 26) % 26 < cribLen
  //
  //
  // floor(criblen / 26) + ((notch1 - r1pos + 26) % 26 < cribLen % 26) =
  // notch1Hits
  //
  // if ((notch2 - r2pos + 26) % 26 < notch1Hits), notch2 will hit
  //
  // NEED to get a range of notches that hit

  // Rotor Loop

  // If strlen(crib) > 26, then we will have repeated notches.
  int fixedCribLen = strlen(crib);

  if (fixedCribLen > 26) {
    fixedCribLen = 26;
  }

  for (int r1 = 0; r1 < 26; ++r1) // rotor 1 position
  {
    for (int r2 = 0; r2 < 26; ++r2) // rotor 2 position
    {
      for (int r3 = 0; r3 < 26; ++r3) // rotor 3 position
      {

        for (int i1 = 0; i1 < fixedCribLen; ++i1) {

          int n1 = (r1 + i1) % 26;

          int notch1Hits =
              (int)(strlen(crib) / 26 + ((n1 - r1 + 26) % 26 < strlen(crib)));

          for (int i2 = 0; i2 <= notch1Hits + 3; ++i2) {

            int n2 = (r2 + i2) % 26;

            // apply settings (needs to be here b/c
            //  		 shortEnigma changes it)
            cfg->r1pos = r1;
            cfg->r2pos = r2;
            cfg->r3pos = r3;

            cfg->notch2 = n2;
            cfg->notch1 = n1;
            // Need to handle the case where the cfg that works is
            // one where the notch doesn't matter

            int out = shortEnigma(crib, cypher, cfg);

            if (out == 1) {

              // reset settings and return

              cfg->r1pos = r1;
              cfg->r2pos = r2;
              cfg->r3pos = r3;

              cfg->notch1 = n1;
              cfg->notch2 = n2;
              return 1;
            }
          }
        }
      }
    }
  }

  // No matching Config found
  return 0;
}

int testPermutation(int rotorNum1, int rotorNum2, int rotorNum3,
                    const struct FastRotors *rotors, struct FastConfig *cfg,
                    char *crib, char *cypher) {
  // set rotors
  cfg->r1 = rotors->r[rotorNum1];
  cfg->r2 = rotors->r[rotorNum2];
  cfg->r3 = rotors->r[rotorNum3];

  cfg->inv_r1 = rotors->inv_r[rotorNum1];
  cfg->inv_r2 = rotors->inv_r[rotorNum2];
  cfg->inv_r3 = rotors->inv_r[rotorNum3];

  return testRotors(crib, cypher, cfg);
}

// 5 * 4 * 3 = 60 rotor Configurations
int permuteRotors(char *crib, char *cypher, const struct FastRotors *rotors,
                  struct FastConfig *cfg) {

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (i == j) {
        continue;
      }

      for (int k = 0; k < 5; ++k) {
        if (k == i || k == j) {
          continue;
        }

        int ret = testPermutation(i, j, k, rotors, cfg, crib, cypher);

        if (ret == 1) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {

  // For development only:

  if (argc != 3) {
    fprintf(stderr, "Invalid Argument count '%d'", argc);
    return 1;
  }

  // make sure that the crib is shorter than the cypher

  // Initalize Rotors

  for (int i = 0; i < 26; ++i) {
    REFLECTOR_1[i] -= 'A';
    REFLECTOR_2[i] -= 'A';
    ALPHABET[i] -= 'A';
    ROTOR_1[i] -= 'A';
    ROTOR_2[i] -= 'A';
    ROTOR_3[i] -= 'A';
    ROTOR_4[i] -= 'A';
    ROTOR_5[i] -= 'A';
  }

  static struct FastRotors rotors;

  rotors.r[0] = ROTOR_1;
  rotors.r[1] = ROTOR_2;
  rotors.r[2] = ROTOR_3;
  rotors.r[3] = ROTOR_4;
  rotors.r[4] = ROTOR_5;

  char INV_R_1[27];
  char INV_R_2[27];
  char INV_R_3[27];
  char INV_R_4[27];
  char INV_R_5[27];

  invertRotor(ROTOR_1, INV_R_1);
  invertRotor(ROTOR_2, INV_R_2);
  invertRotor(ROTOR_3, INV_R_3);
  invertRotor(ROTOR_4, INV_R_4);
  invertRotor(ROTOR_5, INV_R_5);

  rotors.inv_r[0] = INV_R_1;
  rotors.inv_r[1] = INV_R_2;
  rotors.inv_r[2] = INV_R_3;
  rotors.inv_r[3] = INV_R_4;
  rotors.inv_r[4] = INV_R_5;

  rotors.rfl[0] = REFLECTOR_1;
  rotors.rfl[1] = REFLECTOR_2;

  char *crib = (char *)malloc(sizeof(char) * strlen(argv[1]));
  char *cypher = (char *)malloc(sizeof(char) * strlen(argv[1]));

  // since I am only checking if the crib is at the very start of the cypher,
  // I only need the first X characters of the cypher to compare with the crib
  strcpy(crib, argv[1]);
  strncpy(cypher, argv[2], strlen(argv[1]));

  printf("Cypher: '%s'\nCrib: '%s'\n", cypher, crib);

  // shuffle rotors and call testRotors on each

  struct FastConfig cfg;

  // call permute twice, once with each reflector
  cfg.rfl = REFLECTOR_1;

  int ret = permuteRotors(crib, cypher, &rotors, &cfg);

  if (ret == 1) {
    printConfig(&cfg, &rotors);
  } else {
    // if fail, call again with the other rotor
    cfg.rfl = REFLECTOR_2;

    ret = permuteRotors(crib, cypher, &rotors, &cfg);

    if (ret == 1) {
      printConfig(&cfg, &rotors);
    } else {
      printf("No working config found\n");
    }
  }

  free(crib);
  free(cypher);

  return 0;
}
