#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "glad/glad.h"
#include "opengl.h"

uint compile_shader(const void * vp,
                    const int vp_length,
                    const void * fp,
                    const int fp_length)
{
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const char **)&vp, &vp_length);
  glCompileShader(vertex_shader);
  {
    int  success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      fprintf(stderr, "vertex shader compile failed:\n%s\n", info_log);
    }
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const char **)&fp, &fp_length);
  glCompileShader(fragment_shader);
  {
    int  success;
    char info_log[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      fprintf(stderr, "fragment shader compile failed:\n%s\n", info_log);
    }
  }

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glBindAttribLocation(shader_program, 0, "position");
  glLinkProgram(shader_program);

  {
    int  success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      fprintf(stderr, "program link failed:\n%s\n", info_log);
    }
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}

uint make_buffer(unsigned int target,
                 const void * data,
                 size_t size)
{
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, GL_STATIC_DRAW);

  glBindBuffer(target, 0);
  return buffer;
}

uint make_texture(const void * data,
                  int internalformat,
                  int width,
                  int height,
                  int format,
                  int type)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  float color[3] = {0.0, 0.0, 0.0};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

  glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);

  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

uint make_framebuffer(uint * texture, int length)
{
  uint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  for (int i = 0; i < length; i++) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture[i], 0);
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  //printf("glCheckFramebufferStatus: %x\n", status);
  assert(status == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return framebuffer;
}

void debug_framebuffer(int width, int height,
                       int elements,
                       const char * format)
{
  assert(elements == 1 || elements == 3);
  uint gl_format = (elements == 1) ? GL_RED : GL_RGB;

  float out[elements * width * height] = {};
  glReadPixels(0, 0,
               width, height,
               gl_format,
               GL_FLOAT,
               out);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      for (int i = 0; i < elements; i++) {
        printf(format, out[(y * width + x) * elements + i]);
      }
    }
    printf("\n");
  }
}
