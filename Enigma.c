#include <stdio.h>
#include <string.h>

// A char was missing from each of the rotors but I added them back randomly
// Made sure to make the arrays size 26 so there is no null terminator
const char ROTOR_A[26] = "EKMFGHDQVZXRUTIABJSYCLWPON";
const char ROTOR_B[26] = "AJDKSIRUBLHXQTMOWZYVFCPNGE";
const char ROTOR_C[26] = "BDFHJLNPRTVKXZMOQSUWYACEGI";
const char REFLECTOR[26] = "OUIDEFGHCJKLMNAPQRSTBVWXYZ";

const int NOTCH1 = 20;
const int NOTCH2 = 22;

// couldn't find the maps for all 5 of the rotors but they can be easily added

char alphabetIndex(char target) {
  if (target >= 'A' && target <= 'Z') {
    return target - 'A';
  } else {
    return -1;
  }
}

void printRotor(const char *rotor) {
  for (int i = 0; i < 26; ++i) {
    printf("%c", rotor[i] + 'A');
  }
}

/*  // I have no idea why this doesn't work
char
rotorIndex (const char *rotor, char target)
{
  char *ptr = strchr (rotor, target);

  if (ptr != NULL)
    {
      return ptr - rotor;
    }
  else
    {
      return -1;
    }
}
//*/

///*
char rotorIndex(const char *rotor, char target) {
  for (int i = 0; i < 26; ++i) {
    if (target == rotor[i]) {
      return i;
    }
  }

  return -1;
}
//*/

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

void spinRotors(int *rotor1, int *rotor2, int *rotor3) {
  int pos1 = *rotor1;
  int pos2 = *rotor2;
  int pos3 = *rotor3;

  if (NOTCH1 == pos1) {
    pos2 = (pos2 + 1) % 26;
    if (NOTCH2 == pos2) {
      pos3 = (pos3 + 1) % 26;
    }
  }

  pos1 = (pos1 + 1) % 26;

  *rotor1 = pos1;
  *rotor2 = pos2;
  *rotor3 = pos3;
}

int main(int argc, char *argv[]) {

  // As they spin, they increase, so we ADD rotor1_pos to the input and THEN
  // put it into the map
  int rotor1_pos = 0;
  int rotor2_pos = 1;
  int rotor3_pos = 2;
  // Reflector DOES NOT rotate

  char rotor1[26];
  char rotor2[26];
  char rotor3[26];
  char reflector[26];

  memcpy(rotor1, ROTOR_A, sizeof(ROTOR_A));
  memcpy(rotor2, ROTOR_B, sizeof(ROTOR_B));
  memcpy(rotor3, ROTOR_C, sizeof(ROTOR_C));
  memcpy(reflector, REFLECTOR, sizeof(REFLECTOR));

  for (int i = 0; i < 26; ++i) {
    rotor1[i] -= 'A';
    rotor2[i] -= 'A';
    rotor3[i] -= 'A';
    reflector[i] -= 'A';
  }

  // these are all random (just for testing)
  // need to make plugs/rotors command line args after I get it working
  char plugs[][2] = {{1, 3}, {4, 8}, {2, 5}, {6, 20}, {19, 0}};

  char numPlugs = sizeof(plugs) / sizeof(plugs[0]);

  char plugboard[26];
  createPlugboard(plugs, numPlugs, plugboard);

  char output[strlen(argv[1]) + 1];

  // DEBUGGING
  for (int i = 0; i < 26; i++) {
    printf("%d, ", rotor1[i]);
  }
  printf("\n");
  for (int i = 0; i < 26; i++) {
    printf("%d, ", rotor2[i]);
  }
  printf("\n");
  for (int i = 0; i < 26; i++) {
    printf("%d, ", rotor3[i]);
  }
  printf("\n");
  for (int i = 0; i < 26; i++) {
    printf("%d, ", reflector[i]);
  }
  printf("\n");
  // DEBUGGING

  for (int i = 0; i < strlen(argv[1]); i++) {
    /*char letter = input[i];*/
    char letter = argv[1][i];
    char index = alphabetIndex(letter);
    printf("%c = %d ->", letter, index);
    index = plugboard[index];
    printf(" %d ->", index);

    index = rotor1[(index + rotor1_pos) % 26];
    printf(" %d ->", index);
    index = rotor2[(index + rotor2_pos) % 26];
    printf(" %d ->", index);
    index = rotor3[(index + rotor3_pos) % 26];
    printf(" %d ->", index);

    index = reflector[index];
    printf(" %d ->", index);

    index = rotorIndex(rotor3, (index + rotor3_pos) % 26);
    printf(" %d ->", index);
    index = rotorIndex(rotor2, (index + rotor2_pos) % 26);
    printf(" %d ->", index);
    index = rotorIndex(rotor1, (index + rotor1_pos) % 26);
    printf(" %d ->", index);

    index = plugboard[index];
    printf(" %d", index);

    output[i] = index + 'A';
    printf(" = %c", output[i]);

    printf("   r1:%d r2:%d r3:%d\n", rotor1_pos, rotor2_pos, rotor3_pos);

    // spinRotors (&rotor1_pos, &rotor2_pos, &rotor3_pos);
  }

  output[strlen(argv[1])] = '\0';

  printf("In -- '%s'\nOut - '%s'\n", argv[1], output);
  return 0;
}

/*
4, 10, 12, 5, 6, 7, 3, 16, 21, 25, 23, 17, 20, 19, 8, 0, 1, 9, 18, 24, 2, 11,
22, 15, 14, 13, 0, 9, 3, 10, 18, 8, 17, 20, 1, 11, 7, 23, 16, 19, 12, 14, 22,
25, 24, 21, 5, 2, 15, 13, 6, 4, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 10, 23,
25, 12, 14, 16, 18, 20, 22, 24, 0, 2, 4, 6, 8, T = 19 -> 0 -> 12 -> 15 -> 11 ->
4 -> 24 -> 1 -> 5 -> 2 = C C = 2 -> 5 -> 16 -> 0 -> 16 -> 8 -> 6 -> 22 -> 14 ->
14 = O
*/
