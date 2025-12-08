#version 330 core

uniform vec4 in_size; // w h halfpx_w halfpx_h
uniform vec4 out_size; // w h halfpx_w halfpx_h

out vec4 fragment_result;

in vec2 f_position;

vec2 halfpixel(float x, float y, vec4 size)
{
  return vec2((x * 2.0 + 1) * size.z,
              (y * 2.0 + 1) * size.w);
}

vec2 rectangularize(float ix, vec4 size)
{
  float y = floor(ix * size.z * 2.0);
  float x = ix - size.x * y;

  return halfpixel(x, y, size);
}

float get_input(float ix)
{
  return texture(tex_sampler, rectangularize(ix, in_size)).x * 255.0;
}

void main()
{
  float x = 0.0;
  float y = 0.0;
  float ix = 0.0;
}
