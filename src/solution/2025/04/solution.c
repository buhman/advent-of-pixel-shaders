#include <stdio.h>

#include "glad.h"
#include "input.h"
#include "opengl.h"

#include "puzzle/2025/04/input.h"
#include "solution/generic.vs.glsl.h"
#include "solution/2025/04/simulate.fs.glsl.h"
#include "solution/2025/04/count.fs.glsl.h"

static const int temp_width = 256;
static const int temp_height = 256;

static void draw(uint vertex_array,
                 uint framebuffer,
                 uint * texture,
                 int texture_count)
{
  for (int i = 0; i < texture_count; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, temp_width, temp_height);

  const uint draw_buffers0[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers0);

  glBindVertexArray(vertex_array);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void solution_2025_04(unsigned int vertex_array)
{
  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  const char * input_start = puzzle_2025_04_input_start;
  const int input_length = puzzle_2025_04_input_size;
  //const char * input_start = "..@@.@@@@.\n@@@.@.@.@@\n@@@@@.@.@@\n@.@@@@..@.\n@@.@@@@.@@\n.@@@@@@@.@\n.@.@.@.@@@\n@.@@@.@@@@\n.@@@@@@@@.\n@.@.@@@.@.\n";
  //const int input_length = 110;

  int y = 0;
  int x = 0;
  int input_width = 0;
  int input_height = 0;
  float buf[temp_width * temp_height];
  for (int i = 0; i < input_length; i++) {
    char c = input_start[i];
    if (c == '\n') {
      y += 1;
      input_width = x;
      x = 0;
    } else {
      buf[y * temp_width + x] = (float)(int)(c == '@');
      x += 1;
    }
  }
  input_height = y;

  uint texture_input = make_texture(buf,
                                    GL_R32F,
                                    temp_width,
                                    temp_height,
                                    GL_RED,
                                    GL_FLOAT);
  uint texture_a = make_texture(buf,
                                GL_R32F,
                                temp_width,
                                temp_height,
                                GL_RED,
                                GL_FLOAT);
  uint framebuffer_a = make_framebuffer(&texture_a, 1);
  uint texture_b = make_texture(NULL,
                                GL_R32F,
                                temp_width,
                                temp_height,
                                GL_RED,
                                GL_FLOAT);
  uint framebuffer_b = make_framebuffer(&texture_b, 1);

  uint texture_output = make_texture(NULL,
                                     GL_R32F,
                                     1,
                                     1,
                                     GL_RED,
                                     GL_FLOAT);
  uint framebuffer_output = make_framebuffer(&texture_output, 1);

  uint framebuffers[] = { framebuffer_a, framebuffer_b };
  uint textures[] = { texture_a, texture_b };

  //////////////////////////////////////////////////////////////////////
  // shaders
  //////////////////////////////////////////////////////////////////////

  uint p__simulate = compile_shader(src_solution_generic_vs_glsl_start,
                                    src_solution_generic_vs_glsl_size,
                                    src_solution_2025_04_simulate_fs_glsl_start,
                                    src_solution_2025_04_simulate_fs_glsl_size);
  uint p__simulate__input_sampler = glGetUniformLocation(p__simulate, "input_sampler");
  uint p__simulate__dim = glGetUniformLocation(p__simulate, "dim");

  uint p__count = compile_shader(src_solution_generic_vs_glsl_start,
                                    src_solution_generic_vs_glsl_size,
                                    src_solution_2025_04_count_fs_glsl_start,
                                    src_solution_2025_04_count_fs_glsl_size);
  uint p__count__input_sampler_a = glGetUniformLocation(p__count, "input_sampler_a");
  uint p__count__input_sampler_b = glGetUniformLocation(p__count, "input_sampler_b");
  uint p__count__dim = glGetUniformLocation(p__count, "dim");
  uint p__count__input_length = glGetUniformLocation(p__count, "input_length");

  //////////////////////////////////////////////////////////////////////
  // part 1
  //////////////////////////////////////////////////////////////////////
  {
    // simulate
    glUseProgram(p__simulate);
    glUniform1i(p__simulate__input_sampler, 0);
    glUniform4f(p__simulate__dim,
                temp_width,
                temp_height,
                0.5f / temp_width,
                0.5f / temp_height);
    draw(vertex_array, framebuffer_b, &texture_a, 1);
    //debug_framebuffer(input_width, input_height, 1, "%1.0f");

    // count
    glUseProgram(p__count);
    glUniform1i(p__count__input_sampler_a, 0);
    glUniform1i(p__count__input_sampler_b, 1);
    glUniform4f(p__count__dim,
                temp_width,
                temp_height,
                0.5f / temp_width,
                0.5f / temp_height);
    glUniform2f(p__count__input_length,
                input_width,
                input_height);
    uint count_textures[] = {texture_a, texture_b};
    draw(vertex_array, framebuffer_output, count_textures, 2);
    //debug_framebuffer(1, 1, 1, "%f");
    float part1;
    glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, &part1);
    printf("part 1: %d\n", (int)part1);
  }

  //////////////////////////////////////////////////////////////////////
  // part 2
  //////////////////////////////////////////////////////////////////////

  int ix = 0;

  while (true) {
    // simulate
    glUseProgram(p__simulate);
    glUniform1i(p__simulate__input_sampler, 0);
    glUniform4f(p__simulate__dim,
                temp_width,
                temp_height,
                0.5f / temp_width,
                0.5f / temp_height);
    draw(vertex_array, framebuffers[!ix], &textures[ix], 1);
    //debug_framebuffer(input_width, input_height, 1, "%1.0f");

    // count
    glUseProgram(p__count);
    glUniform1i(p__count__input_sampler_a, 0);
    glUniform1i(p__count__input_sampler_b, 1);
    glUniform4f(p__count__dim,
                temp_width,
                temp_height,
                0.5f / temp_width,
                0.5f / temp_height);
    glUniform2f(p__count__input_length,
                input_width,
                input_height);
    uint count_textures[] = {texture_a, texture_b};
    draw(vertex_array, framebuffer_output, count_textures, 2);
    float out;
    glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, &out);
    if (out == 0)
      break;

    ix = !ix;
  }
  // count
  glUseProgram(p__count);
  glUniform1i(p__count__input_sampler_a, 0);
  glUniform1i(p__count__input_sampler_b, 1);
  glUniform4f(p__count__dim,
              temp_width,
              temp_height,
              0.5f / temp_width,
              0.5f / temp_height);
  glUniform2f(p__count__input_length,
              input_width,
              input_height);
  uint count_textures[] = {texture_input, texture_b};
  draw(vertex_array, framebuffer_output, count_textures, 2);
  float part2;
  glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, &part2);
  printf("part 2: %d\n", (int)part2);
}
