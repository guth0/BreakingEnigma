#include "../enigma/enigma.h"
#include "../enigma/rotors.h"
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

int
main (int argc, char *argv[])
{

  // For development only:

  if (argc != 3)
    {
      fprintf (stderr, "Invalid Argument count '%d'", argc);
      return 1;
    }

  char *crib = (char *)malloc (sizeof (char) * strlen (argv[1]));
  char *cypher = (char *)malloc (sizeof (char) * strlen (argv[1]));

  // since I am only checking if the crib is at the very start of the cypher,
  // I only need the first X character of the cypher to compare with the crib
  strcpy (crib, argv[1]);
  strncpy (cypher, argv[2], strlen (argv[1]));
  //

  printf ("Cypher: '%s'\nCrib: '%s'\n", cypher, crib);

  // to increase efficiency, I can't just use the Enigma function I already
  // wrote, I have to harvest the code and wrap it in a for loop

  return 0;
}

// IDEA:
// 	- use testRotors() to find the "next" config that works with the
// 	first letter of the crib and cyphertext
// 	- pass testRotors a starter config to start with so that it can
// 	remmeber where it left off.
// 	- this could just spin the rotors and not change them around, I could
// 	just call this once for every configuration of rotor positions
int
shortEnigma (char *crib, char *cypher, struct config cfg)
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

      //    index = cfg.plugboard[index];

      index = cfg.r1[index];
      index = cfg.r2[index];
      index = cfg.r3[index];

      index = cfg.rfl[index];

      index = rotorIndex (cfg.r3, index);
      index = rotorIndex (cfg.r2, index);
      index = rotorIndex (cfg.r1, index);

      //    index = cfg.plugboard[index];

      if (cypher[i] != index + 'a')
        {
          return 0;
        }

      // either this call is unreadable or the function itself is unreadable :(
      spinRotors (cfg.r1, cfg.r2, cfg.r3, &cfg.r1pos, &cfg.r2pos, &cfg.r3pos,
                  cfg.notch1, cfg.notch2);
    }

  return 1;
}

// since the config is passed by pointer, the function can just return 1 if a
// correct position is found and 0 if not
// optimized enigma function
// 	no plugboard (for now)
// 	stops at the first incorrect letter
// 	returns 1 if encyrpted crib = cypher, 0 if not
// 	can't have cfg passed by pointer
//
// This MUST recieve a cfg that has all rotors at position 0!
int
testRotors (char *crib, char *cypher, struct config *cfg)
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
      rotate (cfg->r1, 1, 26);
      for (int r2 = 0; r2 < 26; ++r2) // rotor 2 position
        {
          rotate (cfg->r2, 1, 26);
          for (int r3 = 0; r3 < 26; ++r3) // rotor 3 position
            {
              rotate (cfg->r3, 1, 26);

              // NOTCHES
              for (int n1 = 0; (n1 - r1 + 26) % 26 <= strlen (crib);
                   ++n1) // notch 1 position
                {
                  int notch1Hits
                      = (int)(strlen (crib) / 26
                              + ((n1 - r1 + 26) % 26 < strlen (crib)));

                  for (int n2 = 0; (n2 - r2 + 26) % 26 < notch1Hits;
                       ++n2) // notch 2 position
                    {

                      int out = shortEnigma (crib, cypher, *cfg);

                      if (out == 1)
                        {
                          return 1;
                        }
                    }
                }
            }
        }
    }

  // Notch Loop

  // No matching config found
  return 0;
}
