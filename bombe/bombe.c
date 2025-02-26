#include "../enigma/enigma.h"
#include "../util/config.h"
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

char ROTOR_1[27] = "EKMFGHDQVZXRUTIABJSYCLWPON";
char ROTOR_2[27] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
char ROTOR_3[27] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
char ROTOR_4[27] = "KCPXSYOMZRTLEVFWUNAIQGHBDJ";
char ROTOR_5[27] = "LZIJTBYNREOAPUVDWHXCKQMGFS";

char REFLECTOR_1[27] = "COAHIJRDEFQZWYBUKGXVPTMSNL";
char REFLECTOR_2[27] = "WKPZGHEFVQBXRYUCJMTSOIALND";

char ALPHABET[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int
shortEnigma (char *crib, char *cypher, struct Config *cfg)
{

  // char *output = (char *)malloc(strlen(string) + 1);

  for (int i = 0; i < strlen (crib); i++)
    {

      char letter = crib[i];
      char index = alphabetIndex (letter);

      // if non-alphabetical:
      if (index == -1)
        {

          if (index != cypher[i])
            {
              return 0;
            }

          continue;
        }

      // index = cfg->plugboard[index];

      index = cfg->r1[(index - cfg->r1pos + 26) % 26];
      index = cfg->r2[(index - cfg->r2pos + 26) % 26];
      index = cfg->r3[(index - cfg->r3pos + 26) % 26];

      index = cfg->rfl[index];

      index = (rotorIndex (cfg->r3, index) + cfg->r3pos) % 26;
      index = (rotorIndex (cfg->r2, index) + cfg->r2pos) % 26;
      index = (rotorIndex (cfg->r1, index) + cfg->r1pos) % 26;

      // index = cfg->plugboard[index];

      if (cypher[i] != index + 'A')
        {
          return 0;
        }

      spinRotors (&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1,
                  cfg->notch2);
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
int
testRotors (char *crib, char *cypher, struct Config *cfg)
{

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

  for (int r1 = 0; r1 < 26; ++r1) // rotor 1 position
    {
      cfg->r1pos = r1;
      for (int r2 = 0; r2 < 26; ++r2) // rotor 2 position
        {
          cfg->r2pos = r2;
          for (int r3 = 0; r3 < 26; ++r3) // rotor 3 position
            {
              cfg->r3pos = r3;

              // NOTCHES
              for (int n1 = 0; (n1 - r1 + 26) % 26 <= strlen (crib);
                   ++n1) // notch 1 position
                {
                  int notch1Hits
                      = (int)(strlen (crib) / 26
                              + ((n1 - r1 + 26) % 26 < strlen (crib)));
                  cfg->notch1 = n1;

                  for (int n2 = 0; (n2 - r2 + 26) % 26 <= notch1Hits;
                       ++n2) // notch 2 position
                    {
                      cfg->notch2 = n2;

                      // Need to handle the case where the cfg that works is
                      // one where the notch doesn't matter

                      int out = shortEnigma (crib, cypher, cfg);

                      if (out == 1)
                        {
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

int
main (int argc, char *argv[])
{

  // For development only:

  if (argc != 3)
    {
      fprintf (stderr, "Invalid Argument count '%d'", argc);
      return 1;
    }

  // Initalize Rotors

  for (int i = 0; i < 26; ++i)
    {
      REFLECTOR_1[i] -= 'A';
      REFLECTOR_2[i] -= 'A';
      ALPHABET[i] -= 'A';
      ROTOR_1[i] -= 'A';
      ROTOR_2[i] -= 'A';
      ROTOR_3[i] -= 'A';
      ROTOR_4[i] -= 'A';
      ROTOR_5[i] -= 'A';
    }

  static struct Rotors rotors;

  rotors.r1 = ROTOR_1;
  rotors.r2 = ROTOR_2;
  rotors.r3 = ROTOR_3;
  rotors.r4 = ROTOR_4;
  rotors.r5 = ROTOR_5;

  rotors.rfl1 = REFLECTOR_1;
  rotors.rfl2 = REFLECTOR_2;

  char *crib = (char *)malloc (sizeof (char) * strlen (argv[1]));
  char *cypher = (char *)malloc (sizeof (char) * strlen (argv[1]));

  // since I am only checking if the crib is at the very start of the cypher,
  // I only need the first X characters of the cypher to compare with the crib
  strcpy (crib, argv[1]);
  strncpy (cypher, argv[2], strlen (argv[1]));

  printf ("Cypher: '%s'\nCrib: '%s'\n", cypher, crib);

  // 5 * 4 * 3 = 60 rotor Configurations

  struct Config cfg;
  cfg.r1 = ROTOR_1;
  cfg.r2 = ROTOR_2;
  cfg.r3 = ROTOR_3;
  cfg.rfl = REFLECTOR_1;

  int ret = testRotors (crib, cypher, &cfg);

  printConfig (&cfg, &rotors);

  return 0;
}
