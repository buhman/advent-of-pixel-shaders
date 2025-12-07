#version 330 core

out vec4 fragment_color;

in vec2 f_position;

void main()
{
  fragment_color = vec4(f_position, 0, 0);
}
