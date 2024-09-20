#include <stdio.h>

const int NUM_ROTORS = 3;
const int NUM_LETTERS = 26;
const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// NOTES:
//   A key can NEVER come back as itself
//   Energy Pathing:
//     Battery -> Switch -> Plugboard -> Rotors -> Plugboard -> Switch -> Lightbulb -> Battery
//   Rotor Spins:
//     Fast Rotor (Right) Always spins on every button press
//     Medium Rotor (Middle) Spins once every 26 fast rotor rotations
//     Slow Rotor (Left) Spins once every 26 medium rotor rotations or 26*26 fast rotor rotations
//   Configurations:
//     Rotors:
//       Order of the Rotors (Machine comes with 5, they can be placed in any order)
//       Position of the Rotors (26 positions for each)
//       Positions of the notches (26 positions for the first two rotors)
//     Plugboard:
//       Any two letters can be swapped into another
//

int main()
{
    char cypher_text[] = "This is the cypher text!!!";

    char plain_text[] = "This is the plain text!!!!";

    int dial1, dial2, dial3 = 0;

    printf("FIN!");

    return 0;
}