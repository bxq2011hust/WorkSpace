// #include <stdio.h>
#include "Common.h"
#include <stdint.h>

EM_PORT_API(int) EMSCRIPTEN_KEEPALIVE
clz_64(int64_t x)
{
  if (x == 0)
    return 64;
  int n = 0;
  if ((x & 0xFFFFFFFF00000000) == 0)
  {
    n += 32;
    x <<= 32;
  }
  if ((x & 0xFFFF000000000000) == 0)
  {
    n += 16;
    x <<= 16;
  }
  if ((x & 0xFF00000000000000) == 0)
  {
    n += 8;
    x <<= 8;
  }
  if ((x & 0xF000000000000000) == 0)
  {
    n += 4;
    x <<= 4;
  }
  if ((x & 0xC000000000000000) == 0)
  {
    n += 2;
    x <<= 2;
  }
  if ((x & 0x8000000000000000) == 0)
  {
    n += 1;
  }
  return n;
}
