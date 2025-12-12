#version 130

uniform sampler2D input_sampler;
uniform vec4 dim; // w h halfpx_w halfpx_h

out vec4 fragment_result;

in vec2 f_position;

vec2 halfpixel(float x, float y, vec4 dim)
{
  return vec2((x * 2.0 + 1.0) * dim.z,
              (y * 2.0 + 1.0) * dim.w);
}

float sample(sampler2D sampler, float x, float y)
{
  return texture(sampler, halfpixel(x, y, dim)).x;
}

float neighbors(sampler2D sampler, float x, float y)
{
  float acc = 0.0;
  acc += sample(sampler, x - 1, y - 1);
  acc += sample(sampler, x + 0, y - 1);
  acc += sample(sampler, x + 1, y - 1);
  acc += sample(sampler, x + 1, y + 0);
  acc += sample(sampler, x - 1, y + 0);
  acc += sample(sampler, x - 1, y + 1);
  acc += sample(sampler, x + 0, y + 1);
  acc += sample(sampler, x + 1, y + 1);
  return acc;
}

vec2 integer_coord(vec2 v, vec4 dim)
{
  return ((v * 0.5 + 0.5) - dim.zw) * dim.xy;
}

void main()
{
  vec2 c = integer_coord(f_position, dim);

  float n = neighbors(input_sampler, c.x, c.y);

  float v = sample(input_sampler, c.x, c.y);

  float result = (n < 4) ? 0 : v;

  fragment_result = vec4(result);
}
