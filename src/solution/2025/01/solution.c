#include <stdio.h>

#include "glad.h"
#include "input.h"
#include "opengl.h"

#include "puzzle/2025/01/input.h"
#include "solution/2025/01/solution.vs.glsl.h"
#include "solution/2025/01/solution.fs.glsl.h"

const int output_width = 1;
const int output_height = 1;

void solution_2025_01(unsigned int vertex_array)
{
  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  const char * input_start = puzzle_2025_01_input_start;
  const int input_length = puzzle_2025_01_input_size;
  int input_width;
  uint texture_input = rectangularize_input(input_start,
                                            input_length,
                                            &input_width);
  int input_height = input_width;

  uint texture_output = make_texture(NULL,
                                     GL_RGBA32F,
                                     output_width,
                                     output_height,
                                     GL_RGBA,
                                     GL_UNSIGNED_BYTE);

  uint framebuffer_output = make_framebuffer(&texture_output, 1);

  //////////////////////////////////////////////////////////////////////
  // shaders
  //////////////////////////////////////////////////////////////////////

  uint program = compile_shader(src_solution_2025_01_solution_vs_glsl_start,
                                src_solution_2025_01_solution_vs_glsl_size,
                                src_solution_2025_01_solution_fs_glsl_start,
                                src_solution_2025_01_solution_fs_glsl_size);
  uint program__input_sampler = glGetUniformLocation(program, "input_sampler");
  uint program__input_dim = glGetUniformLocation(program, "input_dim");
  uint program__input_length = glGetUniformLocation(program, "input_length");

  //////////////////////////////////////////////////////////////////////
  // shader uniforms
  //////////////////////////////////////////////////////////////////////

  glUseProgram(program);
  glUniform1i(program__input_sampler, 0);
  glUniform4f(program__input_dim,
              input_width,
              input_height,
              0.5f / input_width,
              0.5f / input_height);
  glUniform1f(program__input_length, input_length);

  //////////////////////////////////////////////////////////////////////
  // runner
  //////////////////////////////////////////////////////////////////////

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_output);
  glViewport(0, 0, output_width, output_height);

  // clear

  glClearColor(0.78, 0.56, 0.34, 0.12);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_input);
  const uint draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);

  glBindVertexArray(vertex_array);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //////////////////////////////////////////////////////////////////////
  // output
  //////////////////////////////////////////////////////////////////////

  float out[4 * output_width * output_height] = {};
  glReadPixels(0, 0,
               output_width, output_height,
               GL_RGBA,
               GL_FLOAT,
               out);

  for (int y = 0; y < output_height; y++) {
    for (int x = 0; x < output_width; x++) {
      printf("[% 2.3f % 2.3f % 2.3f % 2.3f ] ",
             out[(y * output_width + x) * 4 + 0],
             out[(y * output_width + x) * 4 + 1],
             out[(y * output_width + x) * 4 + 2],
             out[(y * output_width + x) * 4 + 3]);
    }
    printf("\n");
  }
}
