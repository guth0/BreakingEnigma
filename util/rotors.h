#ifndef ROTORS_H
#define ROTORS_H

#include "stdio.h"

void reverse(char *arr, int start, int end)
{
  while (start < end)
  {
    int temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;

    start++;
    end--;
  }
}

void rotate(char *arr, int d, int n)
{
  d %= n;
  reverse(arr, 0, d-1);
  reverse(arr, d, n - 1 );
  reverse(arr, 0, n-1);

  // rotated!! :)
}

char alphabetIndex(char target) {
  if (target >= 'A' && target <= 'Z') {
    return target - 'A';
  } else if (target >= 'a' && target <= 'z') {
    return target - 'a';
  } else {
    return -1;
  }
}

char rotorIndex(const char *rotor, char target) {
  for (int i = 0; i < 26; ++i) {
    if (target == rotor[i]) {
      return i;
    }
  }

  return -1;
}

void printRotor(char *arr)
{
  for (int i = 0; i < 26; ++i)
  {
    printf("%c", arr[i] + 'A');
  }
  
  printf("\n");
}


#endif
