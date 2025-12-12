#version 130

uniform sampler2D input_sampler_a;
uniform sampler2D input_sampler_b;
uniform vec4 dim; // w h halfpx_w halfpx_h

uniform vec2 input_length;

out vec4 fragment_result;

vec2 halfpixel(float x, float y, vec4 dim)
{
  return vec2((x * 2.0 + 1.0) * dim.z,
              (y * 2.0 + 1.0) * dim.w);
}

float sample(sampler2D sampler, float x, float y)
{
  return texture(sampler, halfpixel(x, y, dim)).x;
}

void main()
{
  float acc = 0.0;

  for (float y = 0.0; y < input_length.y; y += 1.0) {
    for (float x = 0.0; x < input_length.x; x += 1.0) {
      acc += sample(input_sampler_a, x, y);
      acc -= sample(input_sampler_b, x, y);
    }
  }

  fragment_result = vec4(acc);
}
