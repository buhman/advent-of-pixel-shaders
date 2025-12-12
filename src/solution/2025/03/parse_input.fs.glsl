#version 130

uniform sampler2D input_sampler;
uniform vec4 input_dim; // w h halfpx_w halfpx_h
uniform float input_length;

uniform vec4 output_dim; // w h halfpx_w halfpx_h

out vec4 fragment_result;

in vec2 f_position;

vec2 halfpixel(float x, float y, vec4 dim)
{
  return vec2((x * 2.0 + 1.0) * dim.z,
              (y * 2.0 + 1.0) * dim.w);
}

vec2 rectangularize(float ix, vec4 dim)
{
  float y = floor(ix * dim.z * 2.0);
  float x = ix - dim.x * y;

  return halfpixel(x, y, dim);
}

float get_input(float ix)
{
  return texture(input_sampler, rectangularize(ix, input_dim)).x * 255.0;
}

vec2 integer_coord(vec2 v, vec4 dim)
{
  return ((v * 0.5 + 0.5) - dim.zw) * dim.xy;
}

const float ascii_newline = 10.0;
const float ascii_zero = 48.0;

float find_character(vec2 v)
{
  float x = 0.0;
  float y = 0.0;
  float ix = 0.0;

  while (ix < input_length) {
    float c = get_input(ix);
    if (x == v.x && y == v.y)
      return c - ascii_zero;
    if (c == ascii_newline) {
      x = 0.0;
      y += 1.0;
    } else {
      x += 1.0;
    }
    ix += 1.0;
  }

  return -1.0;
}

void main()
{
  vec2 v = integer_coord(f_position, output_dim);

  //fragment_result = vec4(v, 0, 0);
  fragment_result = vec4(find_character(v));
}
