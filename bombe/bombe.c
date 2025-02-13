#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Plan (Crib Centered):
// 	1. Crib HAS to exist at the very start of the message
// 	2. Crib can be anywhere in the message
// 		a. This is probably harder than it sounds
// 	3. Multiple Cribs (Idk if this is usefull or not)

int main(int argc, char *argv[]) {

  // For development only:

  if (argc != 3) {
    fprintf(stderr, "Invalid Argument count '%d'", argc);
    return 1;
  }

  char *crib = (char *)malloc(sizeof(char) * strlen(argv[1]));
  char *cypher = (char *)malloc(sizeof(char) * strlen(argv[1]));

  // since I am only checking if the crib is at the very start of the cypher,
  // I only need the first X character of the cypher to compare with the crib
  strcpy(crib, argv[1]);
  strncpy(cypher, argv[2], strlen(argv[1]));
  //

  printf("Cypher: '%s'\nCrib: '%s'\n", cypher, crib);

  // to increase efficiency, I can't just use the Enigma function I already
  // wrote, I have to harvest the code and wrap it in a for loop

  return 0;
}
