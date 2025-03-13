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

int
shortEnigma (char *crib, char *cypher, struct Config *cfg)
{

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

      // Send through Enigma

      // index = cfg->plugboard[index];

      index = cfg->r1[(index - cfg->r1pos + 26) % 26];
      index = cfg->r2[(index - cfg->r2pos + 26) % 26];
      index = cfg->r3[(index - cfg->r3pos + 26) % 26];

      index = cfg->rfl[index]; // reflector

      index = (rotorIndex (cfg->r3, index) + cfg->r3pos) % 26;
      index = (rotorIndex (cfg->r2, index) + cfg->r2pos) % 26;
      index = (rotorIndex (cfg->r1, index) + cfg->r1pos) % 26;

      // index = cfg->plugboard[index];

      //      printf("%c", index + 'A');

      // check for match
      if (cypher[i] != index + 'A')
        {
          //	  printf(" !! :(\n");
          return 0;
        }

      spinRotors (&cfg->r1pos, &cfg->r2pos, &cfg->r3pos, cfg->notch1,
                  cfg->notch2);
    }

  //  printf(" :)\n");

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

  // If strlen(crib) > 26, then we will have repeated notches.
  int fixedCribLen = strlen (crib);

  if (fixedCribLen > 26)
    {
      fixedCribLen = 26;
    }

  for (int r1 = 0; r1 < 26; ++r1) // rotor 1 position
    {
      for (int r2 = 0; r2 < 26; ++r2) // rotor 2 position
        {
          for (int r3 = 0; r3 < 26; ++r3) // rotor 3 position
            {

              // NOTCHES
              //              for (int n1 = 0; (n1 - r1 + 26) % 26 <= strlen
              //              (crib);
              //                   ++n1) // notch 1 position

              for (int i1 = 0; i1 < fixedCribLen; ++i1)
                {

                  int n1 = (r1 + i1) % 26;

                  int notch1Hits
                      = (int)(strlen (crib) / 26
                              + ((n1 - r1 + 26) % 26 < strlen (crib)));

                  //                  for (int n2 = 0; (n2 - r2 + 26) % 26 <=
                  //                  notch1Hits;
                  //                       ++n2) // notch 2 position
                  for (int i2 = 0; i2 <= notch1Hits + 3; ++i2)
                    {

                      int n2 = (r2 + i2) % 26;

                      /*
                      if (r1 == 0 && r2 == 0 && r3 == 0)
                        {
                          printf (
                              "r1: %d, r2: %d, r3: %d, n1 : % d, n2 : % d\n ",
                              r1, r2, r3, n1, n2);
                        }*/

                      // apply settings (needs to be here b/c
                      //  		 shortEnigma changes it)
                      cfg->r1pos = r1;
                      cfg->r2pos = r2;
                      cfg->r3pos = r3;

                      cfg->notch2 = n2;
                      cfg->notch1 = n1;
                      // Need to handle the case where the cfg that works is
                      // one where the notch doesn't matter

                      int out = shortEnigma (crib, cypher, cfg);

                      if (out == 1)
                        {

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

int
testPermutation (int rotorNum1, int rotorNum2, int rotorNum3, char *rotors[],
                 struct Config *cfg, char *crib, char *cypher)
{
  // set rotors
  cfg->r1 = rotors[rotorNum1];
  cfg->r2 = rotors[rotorNum2];
  cfg->r3 = rotors[rotorNum3];

  return testRotors (crib, cypher, cfg);
}

int
permuteRotors (char *crib, char *cypher, char *rotors[], struct Config *cfg)
{

  for (int i = 0; i < 5; ++i)
    {
      for (int j = 0; j < 5; ++j)
        {
          if (i == j)
            {
              continue;
            }

          for (int k = 0; k < 5; ++k)
            {
              if (k == i || k == j)
                {
                  continue;
                }

              // printf ("%d, %d, %d\n", i + 1, j + 1, k + 1);

              int ret = testPermutation (i, j, k, rotors, cfg, crib, cypher);

              // for testing
              return 1;

              if (ret == 1)
                {
                  return 1;
                }
            }
        }
    }
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

  // make sure that the crib is shorter than the cypher

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

  char *rotorArr[5] = { ROTOR_1, ROTOR_2, ROTOR_3, ROTOR_4, ROTOR_5 };

  char *crib = (char *)malloc (sizeof (char) * strlen (argv[1]));
  char *cypher = (char *)malloc (sizeof (char) * strlen (argv[1]));

  // since I am only checking if the crib is at the very start of the cypher,
  // I only need the first X characters of the cypher to compare with the crib
  strcpy (crib, argv[1]);
  strncpy (cypher, argv[2], strlen (argv[1]));

  printf ("Cypher: '%s'\nCrib: '%s'\n", cypher, crib);

  // 5 * 4 * 3 = 60 rotor Configurations

  // shuffle rotors and call testRotors on each

  struct Config cfg;
  cfg.r1 = ROTOR_1;
  cfg.r2 = ROTOR_2;
  cfg.r3 = ROTOR_3;

  // call permute twice, once with each reflector
  cfg.rfl = REFLECTOR_1;

  cfg.r1pos = 0;
  cfg.r2pos = 0;
  cfg.r3pos = 0;

  cfg.notch1 = 5;
  cfg.notch2 = 5;

  printf ("ShortEnigma: %d\n", shortEnigma (crib, cypher, &cfg));

  printf ("TestRotors: %d\n", testRotors (crib, cypher, &cfg));

  printf ("testPermutation: %d\n",
          testPermutation (0, 1, 2, rotorArr, &cfg, crib, cypher));

  // return 0;

  int ret = permuteRotors (crib, cypher, rotorArr, &cfg);

  if (ret == 1)
    {
      printConfig (&cfg, &rotors);
      printf ("1st is right\n");
    }
  else
    {
      // if fail, call again with the other rotor
      cfg.rfl = REFLECTOR_2;

      ret = permuteRotors (crib, cypher, rotorArr, &cfg);

      if (ret == 1)
        {
          printConfig (&cfg, &rotors);

          printf ("2st is right\n");
        }
      else
        {
          printf ("No working config found\n");
        }
    }

  free (crib);
  free (cypher);

  return 0;
}
