#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "guts.h"

// A char was missing from each of the rotors but I added them back randomly
// Made sure to make the arrays size 26 so there is no null terminator
const char ROTOR_A[26] = "EKMFGHDQVZXRUTIABJSYCLWPON";
const char ROTOR_B[26] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
const char ROTOR_C[26] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
const char REFLECTOR[26] = "COAHIJRDEFQZWYBUKGXVPTMSNL";

const int NOTCH1 = 20;
const int NOTCH2 = 22;

void printRotor(const char *rotor) {
  for (int i = 0; i < 26; ++i) {
    printf("%c", rotor[i] + 'A');
  }
}

void createPlug(char *board, char plug1, char plug2) {
  // if plug1 != board[plug1] && plug2 != board[plug2]: plug is invalid
  char temp = board[plug1];
  board[plug1] = board[plug2];
  board[plug2] = temp;
}

// plugs should be a 2*n matrix
void createPlugboard(const char plugs[][2], int numPlugs,
                     char *board) // board will always have size = 26
{
  for (int i = 0; i < 26; i++) {
    board[i] = i;
  }

  for (int i = 0; i < numPlugs; i++) {
    const char plug1 = plugs[i][0];
    const char plug2 = plugs[i][1];

    createPlug(board, plug1, plug2);
  }
}

int main(int argc, char *argv[]) {

  char rotor1[] = "EKMFGHDQVZXRUTIABJSYCLWPON";
  char rotor2[] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
  char rotor3[] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
  char reflector[] = "COAHIJRDEFQZWYBUKGXVPTMSNL";

  for (int i = 0; i < 26; ++i) {
    rotor1[i] -= 'A';
    rotor2[i] -= 'A';
    rotor3[i] -= 'A';
    reflector[i] -= 'A';
  }

  if (argc < 2) {
    fprintf(stderr, "Invalid argument count");
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'p':
        // handle Plugboad
        break;
      case 'r':
        // handle Rotors
        break;
      case 'h':
        // give help
        break;
      default:
        printf("Invalid Flag ");
	break;
      }
    }
  }

  //  char *Enigma(char *string, char *r1, char *r2, char *r3, char *rfl, int
  //  r1pos, int r2pos, int r3pos, char plugboard[26], const int notch1,
  //        const int notch2) {

  char *string;

  Enigma();

  return 0;
}
