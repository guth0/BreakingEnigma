#include <stdio.h>

#include "enigma.h"
#include "rotors.h"

const char ROTOR_1[27] = "EKMFGHDQVZXRUTIABJSYCLWPON";
const char ROTOR_2[27] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
const char ROTOR_3[27] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
const char ROTOR_4[27] = "KCPXSYOMZRTLEVFWUNAIQGHBDJ";
const char ROTOR_5[27] = "LZIJTBYNREOAPUVDWHXCKQMGFS";

const char REFLECTOR_1[27] = "COAHIJRDEFQZWYBUKGXVPTMSNL";
const char REFLECTOR_2[27] = "WKPZGHEFVQBXRYUCJMTSOIALND";

const char ALPHABET[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void assignRotor(char *r, int rNum) {

  switch (rNum) {
  case 1:
    strncpy(r, ROTOR_1, 27);
    break;
  case 2:
    strncpy(r, ROTOR_2, 27);
    break;
  case 3:
    strncpy(r, ROTOR_3, 27);
    break;
  case 4:
    strncpy(r, ROTOR_4, 27);
    break;
  case 5:
    strncpy(r, ROTOR_5, 27);
    break;
  }
}

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

char parseArgs(int argc, char *argv[], struct config *cfg, char *string)
{
  int *ptr; // temp variable used for switch cases

  for (int i = 1; i < argc; ++i) {
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

        strncpy(cfg->plugboard, argv[i + 1], 26);
        i++;
        break;
      case 'r':
        for (int j = 0; j < 3; ++j) {
          if (argv[i + 1][j] > '5' || argv[i + 1][j] < '1') {
            fprintf(stderr, "Invalid Rotor Number '%c'\n", argv[i + 1][j]);
            return 3;
          }
        }

        assignRotor(cfg->r1, argv[i + 1][0] - '0');
        assignRotor(cfg->r2, argv[i + 1][1] - '0');
        assignRotor(cfg->r3, argv[i + 1][2] - '0');

        i++;
        break;
      case 'e':

        switch (argv[i + 1][0]) {
        case '1':
          strncpy(cfg->rfl, REFLECTOR_1, 26);
          break;
        case '2':
          strncpy(cfg->rfl, REFLECTOR_2, 26);
          break;
        default:
          fprintf(stderr, "Invalid Reflector Number '%c'\n", argv[i + 1][0]);
          return 4;
        }

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
          fprintf(stderr, "Invalid Rotor Position Format '%s'\n", argv[i + 1]);
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

        i++;
        break;
      case 'v':
        cfg->verbose = 1;
        break;
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
            "[0-25]] [--config=file] [-o file] [-f file] [text]\n"
            "\n"
            "enigma is a recreation of the encryption tool used during WWII\n"
            "\n"
            "Command options:\n"
            "  -r [1-5](3)		choose which of the 5 rotors\n"
	    "			will be used in each slot\n"
            "			  default is '-r 123'\n"
            "  -e [1-2]		choose which of the 2 reflectors to use\n"
	    "			  default is '-e 1'\n"
            "  -b [CYPHER]		define the CYPHER for the plugboard\n"
            "			   default is the normal alphabet\n"
            "  -p1/2/3 [0-25]	each rotor can be in 1 of 26 rotations\n"
            "			  default is '-pX 0'\n"
            "  -n1/2 [0-25]		rotors 1 & 2 have notches which\n"
            "		        can be in 1 of 26 positions\n"
            "			  default is '-n1 5 -n2 10'\n"
            "  --config=file		select a file to load configurations "
            "from\n"
	    "  -v			verbose output\n"
            "  -o 			select a file to use as output\n"
            "  -f 			select a file to load as input\n"
            "  -h			show this help message\n");
        return 1;
        break;
      default:
        printf("Invalid flag '%s'\nUse 'enigma -h' for useage\n", argv[i]);
        break;
      }
    } else // the only other case is that it is the input string
    {
      // this isn't safe but I will fix later
      string = (char *)malloc(strlen(argv[i]) * sizeof(char));
      strcpy(string, argv[i]);
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Invalid argument count\n");
    return 1;
  }

  // give these all default values but overwrite them with user input

  char *string;

  struct config cfg;

  // copy in default cyphers
  strncpy(cfg.r1, ROTOR_1, 27);
  strncpy(cfg.r2, ROTOR_2, 27);
  strncpy(cfg.r3, ROTOR_3, 27);
  strncpy(cfg.rfl, REFLECTOR_1, 27);
  strncpy(cfg.plugboard, ALPHABET, 27);

  // Random Default Values
  cfg.r1pos = 0;
  cfg.r2pos = 0;
  cfg.r3pos = 0;
  cfg.notch1 = 5;
  cfg.notch2 = 10;


  // flags
  cfg.verbose = 0;

  char retVal = parseArgs(argc, argv, &cfg, string);

  if(retVal != 0)
  {
    return retVal;
  }

  for (int i = 0; i < 26; ++i) {
    cfg.r1[i] -= 'A';
    cfg.r2[i] -= 'A';
    cfg.r3[i] -= 'A';
    cfg.rfl[i] -= 'A';
    cfg.plugboard[i] -= 'A';
  }

  if (cfg.r1pos != 0) {
    rotate(cfg.r1, cfg.r1pos, 26);
  }
  if (cfg.r2pos != 0) {
    rotate(cfg.r2, cfg.r2pos, 26);
  }
  if (cfg.r3pos != 0) {
    rotate(cfg.r3, cfg.r3pos, 26);
  }

  if (cfg.verbose != 0) {
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
  }

  char *output;
  output = Enigma(string, cfg);

  printf("%s\n", output);

  free(string);
  free(output);

  return 0;
}
