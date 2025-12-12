#include <stdio.h>

#include "glad/glad.h"
#include "input.h"
#include "opengl.h"

#include "puzzle/2025/03/input.h"
#include "solution/generic.vs.glsl.h"
#include "solution/2025/03/parse_input.fs.glsl.h"
#include "solution/2025/03/solution.fs.glsl.h"

static const int output_width = 1;
static const int output_height = 1;

static const int parsed_input_width = 128;
static const int parsed_input_height = 256;

void solution_2025_03(unsigned int vertex_array)
{
  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  const char * input_start = puzzle_2025_03_input_start;
  const int input_length = puzzle_2025_03_input_size;
  int input_width;
  uint texture_input = rectangularize_input(input_start,
                                            input_length,
                                            &input_width);
  int input_height = input_width;

  uint texture_parsed_input = make_texture(NULL,
                                           GL_R32F,
                                           parsed_input_width,
                                           parsed_input_height,
                                           GL_RED,
                                           GL_UNSIGNED_BYTE);
  uint framebuffer_parsed_input = make_framebuffer(&texture_parsed_input, 1);


  uint texture_output = make_texture(NULL,
                                     GL_RGB32F,
                                     output_width,
                                     output_height,
                                     GL_RGB,
                                     GL_UNSIGNED_BYTE);
  uint framebuffer_output = make_framebuffer(&texture_output, 1);

  //////////////////////////////////////////////////////////////////////
  // shaders
  //////////////////////////////////////////////////////////////////////

  uint p__parse_input = compile_shader(src_solution_generic_vs_glsl_start,
                                       src_solution_generic_vs_glsl_size,
                                       src_solution_2025_03_parse_input_fs_glsl_start,
                                       src_solution_2025_03_parse_input_fs_glsl_size);
  uint p__parse_input__input_sampler = glGetUniformLocation(p__parse_input, "input_sampler");
  uint p__parse_input__input_dim = glGetUniformLocation(p__parse_input, "input_dim");
  uint p__parse_input__input_length = glGetUniformLocation(p__parse_input, "input_length");
  uint p__parse_input__output_dim = glGetUniformLocation(p__parse_input, "output_dim");

  uint p__solution = compile_shader(src_solution_generic_vs_glsl_start,
                                    src_solution_generic_vs_glsl_size,
                                    src_solution_2025_03_solution_fs_glsl_start,
                                    src_solution_2025_03_solution_fs_glsl_size);
  uint p__solution__input_sampler = glGetUniformLocation(p__solution, "input_sampler");
  uint p__solution__input_dim = glGetUniformLocation(p__solution, "input_dim");
  uint p__solution__input_length = glGetUniformLocation(p__solution, "input_length");

  //////////////////////////////////////////////////////////////////////
  // runner step 1: parse input
  //////////////////////////////////////////////////////////////////////

  // shader uniforms

  glUseProgram(p__parse_input);
  glUniform1i(p__parse_input__input_sampler, 0);
  glUniform4f(p__parse_input__input_dim,
              input_width,
              input_height,
              0.5f / input_width,
              0.5f / input_height);
  glUniform1f(p__parse_input__input_length, input_length);

  glUniform4f(p__parse_input__output_dim,
              parsed_input_width,
              parsed_input_height,
              0.5f / parsed_input_width,
              0.5f / parsed_input_height);

  // draw

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_parsed_input);
  glViewport(0, 0, parsed_input_width, parsed_input_height);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_input);
  const uint draw_buffers0[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers0);

  glBindVertexArray(vertex_array);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // debug

  if (1) {
    float out[1 * parsed_input_width * parsed_input_height] = {};
    glReadPixels(0, 0,
                 parsed_input_width, parsed_input_height,
                 GL_RED,
                 GL_FLOAT,
                 out);

    for (int y = 0; y < parsed_input_height; y++) {
      for (int x = 0; x < parsed_input_width; x++) {
        printf("%d", (int)out[(y * parsed_input_width + x) * 1 + 0]);
      }
      printf("\n");
      break;
    }
  }

  //////////////////////////////////////////////////////////////////////
  // runner step 2: solve
  //////////////////////////////////////////////////////////////////////

  // shader uniforms

  glUseProgram(p__solution);
  glUniform1i(p__solution__input_sampler, 0);
  glUniform4f(p__solution__input_dim,
              parsed_input_width,
              parsed_input_height,
              0.5f / parsed_input_width,
              0.5f / parsed_input_height);
  glUniform2f(p__solution__input_length,
              100,
              200); // is this cheating?

  // draw

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_output);
  glViewport(0, 0, output_width, output_height);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_parsed_input);
  const uint draw_buffers1[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers1);

  glBindVertexArray(vertex_array);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //////////////////////////////////////////////////////////////////////
  // output
  //////////////////////////////////////////////////////////////////////

  float out[3 * output_width * output_height] = {};
  glReadPixels(0, 0,
               output_width, output_height,
               GL_RGB,
               GL_FLOAT,
               out);
  printf("2025/03: %2.3f %2.3f\n", out[0], (double)out[1] * 16777216.0 + (double)out[2]);
}
