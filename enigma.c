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

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Invalid argument count\n");
    return 1;
  }

  // give these all default values but overwrite them with user input

  char *string;

  // 27 so there is a space for a null terminator
  char r1[27];
  char r2[27];
  char r3[27];
  char rfl[27];

  // copy in default cyphers
  strncpy(r1, ROTOR_1, 27);
  strncpy(r2, ROTOR_2, 27);
  strncpy(r3, ROTOR_3, 27);
  strncpy(rfl, REFLECTOR_1, 27);

  // Random Default Values
  int r1pos = 0;
  int r2pos = 1;
  int r3pos = 2;

  char plugboard[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  int notch1 = 5;
  int notch2 = 10;

  int *ptr; // temp variable used for switch cases

  // flags
  char verbose = 0;

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

        strncpy(plugboard, argv[i + 1], 26);
        i++;
        break;
      case 'r':
        for (int j = 0; j < 3; ++j) {
          if (argv[i + 1][j] > '5' || argv[i + 1][j] < '1') {
            fprintf(stderr, "Invalid Rotor Number '%c'\n", argv[i + 1][j]);
            return 3;
          }
        }

        assignRotor(r1, argv[i + 1][0] - '0');
        assignRotor(r2, argv[i + 1][1] - '0');
        assignRotor(r3, argv[i + 1][2] - '0');

        i++;
        break;
      case 'e':

        switch (argv[i + 1][0]) {
        case '1':
          strncpy(rfl, REFLECTOR_1, 26);
          break;
        case '2':
          strncpy(rfl, REFLECTOR_2, 26);
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
          ptr = &r1pos;
          break;
        case '2':
          ptr = &r2pos;
          break;
        case '3':
          ptr = &r3pos;
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
          ptr = &notch1;
          break;
        case '2':
          ptr = &notch2;
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
        verbose = 1;
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

        printf("Sorry I haven't made it yet :(\n");
        return 0;
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

  for (int i = 0; i < 26; ++i) {
    r1[i] -= 'A';
    r2[i] -= 'A';
    r3[i] -= 'A';
    rfl[i] -= 'A';
    plugboard[i] -= 'A';
  }

  if (r1pos != 0) {
    rotate(r1, r1pos, 26);
  }
  if (r2pos != 0) {
    rotate(r2, r2pos, 26);
  }
  if (r3pos != 0) {
    rotate(r3, r3pos, 26);
  }

  if (verbose != 0) {
    printf("String: %s\n", string);
    printf("r1 - %d: ", r1pos);
    printCypher(r1);
    printf("r2 - %d: ", r2pos);
    printCypher(r2);
    printf("r3 - %d: ", r3pos);
    printCypher(r3);
    printf("rfl: ");
    printCypher(rfl);
    printf("PB: ");
    printCypher(plugboard);
    printf("n1: %d, n2: %d\n", notch1, notch2);
  }

  char *output;
  output = Enigma(string, r1, r2, r3, rfl, r1pos, r2pos, r3pos, plugboard,
                  notch1, notch2);

  printf("%s\n", output);

  free(string);
  free(output);

  return 0;
}

/*
usage: enigma [-r [1-5](3)] [-p1/2/3 [0-25]] [-e [1-2]] [-b=cypher] [-n1/2
[0-25]] [--config=file] [-f file ...] [text]
*/
