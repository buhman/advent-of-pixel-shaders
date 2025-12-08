#version 330 core

out vec4 fragment_color;

uniform sampler2D tex_sampler;
uniform vec4 tex_size; // w h halfpx_w halfpx_h
uniform float input_length;

vec2 rectangular_position(float ix)
{
  float y = floor(ix * tex_size.z * 2.0);
  float x = ix - tex_size.x * y;

  return vec2((x * 2.0 + 1) * tex_size.z,
              (y * 2.0 + 1) * tex_size.w);
}

const float ascii_zero = 48.0;
const float ascii_nine = 57.0;
const float ascii_l = 76.0;

float get_input(float ix)
{
  return texture(tex_sampler, rectangular_position(ix)).x * 255.0;
}

vec2 parse_integer(float ix)
{
  float number = 0;

  while (ix < input_length) {
    float c = get_input(ix);
    if (c < ascii_zero || c > ascii_nine) {
      return vec2(ix + 1.0, number);
    }
    float digit = c - ascii_zero;
    number = number * 10.0 + digit;
    ix += 1.0;
  }
}

vec2 parse_direction(float ix)
{
  float c = get_input(ix);
  float direction = (c == ascii_l) ? -1.0 : 1.0;
  return vec2(ix + 1.0, direction);
}

vec3 simulate_movement(float ix, float position)
{
  vec2 dir_result = parse_direction(ix);
  ix = dir_result.x;
  float direction = dir_result.y;

  vec2 int_result = parse_integer(ix);
  ix = int_result.x;
  float number = int_result.y;

  float old_position = position;
  float crossings = floor(number / 100.0);
  position += direction * mod(number, 100.0);
  if (position < 0.0) {
    position += 100.0;
    crossings += float(old_position != 0.0);
  }
  if (position > 99.0) {
    position -= 100.0;
    crossings += float(position != 0.0);
  }
  crossings += float(position == 0.0);

  return vec3(ix, position, crossings);
}

void main()
{
  float ix = 0.0;
  float position = 50.0;
  float zeros = 0.0;
  float zero_crossings = 0.0;

  while (ix < input_length) {
    vec3 result = simulate_movement(ix, position);
    ix = result.x;
    position = result.y;
    zero_crossings += result.z;

    if (position == 0.0) {
      zeros += 1.0;
    }
  }

  fragment_color = vec4(zeros, zero_crossings, 0, 0);
}
