#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "glad.h"
#include "opengl.h"

static inline int next_power_of_two(double n)
{
  return pow(2, ceil(log2(n)));
}

uint rectangularize_input(const char * buf, int size, int * width_out)
{
  int width = next_power_of_two(sqrt(size));

  void * tmp = malloc(width * width);
  memcpy(tmp, buf, size);

  uint texture = make_texture(buf,
                              GL_R8, // internalformat
                              width,
                              width,
                              GL_RED,
                              GL_UNSIGNED_BYTE);
  *width_out = width;

  free(tmp);

  return texture;
}
