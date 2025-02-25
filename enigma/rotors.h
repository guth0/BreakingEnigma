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

void printRotor(char *arr)
{
  for (int i = 0; i < 26; ++i)
  {
    printf("%c", arr[i] + 'A');
  }
  
  printf("\n");
}


#endif
