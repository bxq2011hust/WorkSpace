// #include <stdio.h>
#include "Common.h"
#include "stdlib.h"

EM_PORT_API(int)
deploy(int c)
{
  // printf("hello, world!\n");
  int * a = malloc(c);
  for (int i = 1; i < c; i++)
  {
    a[i] = i * a[i];
  }
  return a;
}