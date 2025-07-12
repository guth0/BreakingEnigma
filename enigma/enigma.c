#include <stdio.h>

#include "../util/config.h"
#include "enigma.h"

char isValidPlugboard(char *arr) {
  for (int i = 0; i < 26; ++i) {
    char c = arr[i];
    if ((arr[c - 'A'] != i + 'A') && (c != i + 'A')) {
      return 1;
    }
  }
  return 0;
}

// does not work for negative or decimal numbers
char isNumber(char *arr) {
  for (int i = 0; i < strlen(arr); ++i) {
    if (arr[i] > '9' || arr[i] < '0') {
      return 1;
    }
  }

  return 0;
}

void printCypher(const char *c) {
  for (int i = 0; i < 26; ++i) {
    printf("%c", c[i] + 'A');
  }
  printf("\n");
}

char parseArgs(int argc, char *argv[], struct Config *cfg, char **string,
               struct Rotors *rotors) {
  int *ptr;  // temp variable used for switch cases

  for (int i = 1; i < argc; ++i) {
    //    printf("ARGUMENT: %s", argv[i]);

    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        case 'b':
          if (strlen(argv[i + 1]) != 26) {
            fprintf(stderr, "Invalid Pugboard Cypher Length '%lu'\n",
                    strlen(argv[i + 1]));
            return 2;
          }
          if (isValidPlugboard(argv[i + 1]) != 0) {
            fprintf(stderr, "Pluboard Cypher is Invalid\n");
            return 2;
          }

          memcpy(cfg->plugboard, argv[i + 1], 26);

          // since two arguemnts are used, we increment i an extra time
          i++;
          break;
        case 'r':
          for (int j = 0; j < 3; ++j) {
            if (argv[i + 1][j] > '5' || argv[i + 1][j] < '1') {
              fprintf(stderr, "Invalid Rotor Number '%c'\n", argv[i + 1][j]);
              return 3;
            }
          }

          // sets the rotors to the selected ones
          memcpy(cfg->r1, rotors->r[argv[i + 1][0] - '1'], 27);
          memcpy(cfg->r2, rotors->r[argv[i + 1][1] - '1'], 27);
          memcpy(cfg->r3, rotors->r[argv[i + 1][2] - '1'], 27);

          // since two arguemnts are used, we increment i an extra time
          i++;
          break;
        case 'e':

          switch (argv[i + 1][0]) {
            case '1':
            case '2':

              // changes the rotor to the selected one
              memcpy(cfg->rfl, rotors->rfl[argv[i + 1][0] - '1'], 27);

              break;
            default:
              fprintf(stderr, "Invalid Reflector Number '%c'\n",
                      argv[i + 1][0]);
              return 4;
          }

          // since two arguemnts are used, we increment i an extra time
          i++;
          break;
        case 'p':
          switch (argv[i][2]) {
            case '1':
              ptr = &cfg->r1pos;
              break;
            case '2':
              ptr = &cfg->r2pos;
              break;
            case '3':
              ptr = &cfg->r3pos;
              break;
            default:
              fprintf(stderr, "Invalid Rotor Number '%s'\n", argv[i]);
              return 5;
          }

          if (isNumber(argv[i + 1]) != 0) {
            fprintf(stderr, "Invalid Rotor Position Format '%s'\n",
                    argv[i + 1]);
            return 5;
          }

          *ptr = atoi(argv[i + 1]);

          if (*ptr > 25 || *ptr < 0) {
            fprintf(stderr, "Invalid Rotor Position '%d'\n", *ptr);
            return 5;
          }
          break;
        case 'n':

          switch (argv[i][2]) {
            case '1':
              ptr = &cfg->notch1;
              break;
            case '2':
              ptr = &cfg->notch2;
              break;
            default:
              fprintf(stderr, "Invalid Notch Number '%s'\n", argv[i]);
              return 6;
          }

          if (isNumber(argv[i + 1]) != 0) {
            fprintf(stderr, "Invalid Notch Position '%s'\n", argv[i + 1]);
            return 6;
          }

          *ptr = atoi(argv[i + 1]);

          if (*ptr > 25 || *ptr < 0) {
            fprintf(stderr, "Invalid Notch Position '%d'\n", *ptr);
            return 6;
          }

          // since two arguemnts are used, we increment i an extra time
          i++;
          break;
          //      case 'v':
          //       cfg->verbose = 1;
          //       break;
        case 'f':
          fprintf(stderr, "File input is not yet supported\n");
          return 10;

          break;
        case '-':
          fprintf(stderr, "Config files are not yet supported\n");
          return 10;
          break;
        case 'h':

          printf(
              "usage: enigma [-r [1-5](3)] [-p1/2/3 [0-25]] [-e [1-2]] "
              "[-b=cypher] [-n1/2\n"
              "[0-25]] [--Config=file] [-o file] [-f file] [text]\n"
              "\n"
              "enigma is a recreation of the encryption tool used during "
              "WWII\n"
              "\n"
              "Command options:\n"
              "  -r [1-5](3)		choose which of the 5 rotors\n"
              "			will be used in each slot\n"
              "			  default is '-r 123'\n"
              "  -e [1-2]		choose which of the 2 reflectors to "
              "use\n"
              "			  default is '-e 1'\n"
              "  -b [CYPHER]		define the CYPHER for the "
              "plugboard\n"
              "			   default is the normal alphabet\n"
              "  -p1/2/3 [0-25]	each rotor can be in 1 of 26 "
              "rotations\n"
              "			  default is '-pX 0'\n"
              "  -n1/2 [0-25]		rotors 1 & 2 have notches "
              "which\n"
              "		        can be in 1 of 26 positions\n"
              "			  default is '-n1 5 -n2 10'\n"
              //"  --Config=file		select a file to load "
              //"Configurations "
              //"from\n"
              //	    "  -v			verbose output\n"
              //"  -o 			select a file to use as "
              //"output\n"
              //"  -f 			select a file to load as "
              //"input\n"
              "  -h			show this help message\n");
          return 1;
          break;
        default:
          printf("Invalid flag '%s'\nUse 'enigma -h' for useage\n", argv[i]);
          break;
      }
    } else  // the only other case is that it is the input string
    {
      // this isn't safe but I will fix later
      *string = (char *)malloc(strlen(argv[i]) * sizeof(char));
      strcpy(*string, argv[i]);
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Invalid argument count\n");
    return 1;
  }

  // Initalize Rotors
  char ROTOR_1[27] = "EKMFGHDQVZXRUTIABJSYCLWPON";
  char ROTOR_2[27] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
  char ROTOR_3[27] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
  char ROTOR_4[27] = "KCPXSYOMZRTLEVFWUNAIQGHBDJ";
  char ROTOR_5[27] = "LZIJTBYNREOAPUVDWHXCKQMGFS";

  char REFLECTOR_1[27] = "COAHIJRDEFQZWYBUKGXVPTMSNL";
  char REFLECTOR_2[27] = "WKPZGHEFVQBXRYUCJMTSOIALND";

  char ALPHABET[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

  static struct Rotors rotors;

  rotors.r[0] = ROTOR_1;
  rotors.r[1] = ROTOR_2;
  rotors.r[2] = ROTOR_3;
  rotors.r[3] = ROTOR_4;
  rotors.r[4] = ROTOR_5;

  rotors.rfl[0] = REFLECTOR_1;
  rotors.rfl[1] = REFLECTOR_2;

  // give these all default values but overwrite them with user input

  char *string;

  struct Config cfg;

  // copy in default cyphers
  memcpy(cfg.r1, ROTOR_1, 27);
  memcpy(cfg.r2, ROTOR_2, 27);
  memcpy(cfg.r3, ROTOR_3, 27);
  memcpy(cfg.rfl, REFLECTOR_1, 27);
  memcpy(cfg.plugboard, ALPHABET, 27);

  // Random Default Values
  cfg.r1pos = 0;
  cfg.r2pos = 0;
  cfg.r3pos = 0;
  cfg.notch1 = 5;
  cfg.notch2 = 10;

  // flags
  //  cfg.verbose = 0;

  char retVal = parseArgs(argc, argv, &cfg, &string, &rotors);

  // create inverted rotors
  invertConfig(&cfg);

  if (retVal != 0) {
    return retVal;
  }

  /*  if (cfg.verbose != 0) {
      printf("String: %s\n", string);
      printf("r1 - %d: ", cfg.r1pos);
      printCypher(cfg.r1);
      printf("r2 - %d: ", cfg.r2pos);
      printCypher(cfg.r2);
      printf("r3 - %d: ", cfg.r3pos);
      printCypher(cfg.r3);
      printf("rfl: ");
      printCypher(cfg.rfl);
      printf("PB: ");
      printCypher(cfg.plugboard);
      printf("n1: %d, n2: %d\n", cfg.notch1, cfg.notch2);
    } */

  char *output;
  output = Enigma(string, &cfg);

  printf("%s\n", output);

  free(string);
  free(output);

  return 0;
}
