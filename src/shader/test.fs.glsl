#version 330 core

out vec4 fragment_color;

uniform sampler2D tex_sampler;
uniform vec4 tex_size;

in vec2 f_position;

void main()
{
  vec4 p = texture(tex_sampler, f_position);

  float sum = 0;
  for (float y = 0; y < tex_size.y; y++) {
    for (float x = 0; x < tex_size.x; x++) {
      vec2 coord = vec2((x * 2 + 1) * tex_size.z,
                        (y * 2 + 1) * tex_size.w);

      sum += texture(tex_sampler, coord).x * 255;
    }
  }

  fragment_color = vec4(p.x * 255, sum, f_position);
}
