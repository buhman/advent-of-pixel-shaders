#version 130

in vec2 position;

out vec2 f_position;

void main()
{
  f_position = position;

  gl_Position = vec4(position, 0, 1);
}
