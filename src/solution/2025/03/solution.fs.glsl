#version 130

uniform sampler2D input_sampler;
uniform vec4 input_dim; // w h halfpx_w halfpx_h

uniform vec2 input_length;

out vec4 fragment_result;

vec2 halfpixel(float x, float y, vec4 dim)
{
  return vec2((x * 2.0 + 1.0) * dim.z,
              (y * 2.0 + 1.0) * dim.w);
}

float get_input(float x, float y)
{
  return texture(input_sampler, halfpixel(x, y, input_dim)).x;
}

vec2 max_single(float y, float start, float end)
{
  float max_v = get_input(start, y);
  float max_ix = start;
  float x = max_ix + 1.0;
  while (x < end) {
    float value = get_input(x, y);
    if (value > max_v) {
      max_v = value;
      max_ix = x;
    }
    x += 1.0;
  }
  return vec2(max_v, max_ix);
}

double powd(float n)
{
  double res = double(1.0);
  for (float i = 0.0; i < n; i++) {
    res *= double(10.0);
  }
  return res;
}

double max_joltage_n(float y, float reserve)
{
  float start = 0.0;
  double acc = double(0.0);
  while (reserve > 0.0) {
    reserve -= 1.0;
    vec2 res = max_single(y, start, input_length.x - reserve);
    float d = res.x;
    float ix = res.y;
    start = ix + 1.0;
    acc += powd(reserve) * double(d);
  }
  return acc;
}

void main()
{
  double acc_part1 = double(0.0);
  for (float i = 0.0; i < input_length.y; i += 1.0) {
    acc_part1 += max_joltage_n(i, 2);
  }

  double acc_part2 = double(0.0);
  for (float i = 0.0; i < input_length.y; i += 1.0) {
    acc_part2 += max_joltage_n(i, 12);
  }

  double radix = double(16777216.0);
  double h = double(floor(float(acc_part2 / radix)));
  double l = acc_part2 - h * radix;

  fragment_result = vec4(float(acc_part1), h, l, 0);
}
