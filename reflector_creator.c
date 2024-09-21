#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

static void
shuffle (char *array, size_t n)
{
  char *tmp;
  char *arr = array;
  size_t stride = sizeof (char);
  size_t size = 1;

  if (n > 1)
    {
      size_t i;
      for (i = 0; i < n - 1; ++i)
        {
          size_t rnd = (size_t)rand ();
          size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

	  strcpy(tmp, arr + j * stride);   
	  // memcpy (tmp, arr + j * stride, size);
          memcpy (arr + j * stride, arr + i * stride, size);
          memcpy (arr + i * stride, tmp, size);
        }
    }
}
int
main (int argc, char *argv[])
{
  char *reflector = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  shuffle(reflector, 20);

  printf ("\"");
  for (int i = 0; i < 26; ++i)
    {
      printf ("%c", reflector[i]);
    }
  printf ("\"");
  
  free(reflector);

  return 0;
}
