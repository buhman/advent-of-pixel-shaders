#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include "opengl.h"

#include "shader/test.vs.glsl.h"
#include "shader/test.fs.glsl.h"

int vp_width = 800;
int vp_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  vp_width = width;
  vp_height = height;
}

const float triangle_array[] = {
  -1, -1,
   1, -1,
  -1,  1,
   1,  1
};

int main()
{
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(vp_width, vp_height, "advent-of-pixel-shaders", NULL, NULL);
  if (window == NULL) {
    const char* description;
    glfwGetError(&description);
    fprintf(stderr, "Failed to create GLFW window: %s\n", description);
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "gladLoadGLLoader error\n");
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  const int buf_width = 4;
  const int buf_height = 4;

  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  const char * data =
    "arst"
    "qwfp"
    "1234"
    "6789";
  int input_width = 4;
  int input_height = 4;

  uint texture_input = make_texture(data,
                                    GL_R8, // internalformat
                                    input_width,
                                    input_height,
                                    GL_RED,
                                    GL_UNSIGNED_BYTE);

  uint texture_framebuffer = make_texture(NULL,
                                          GL_RGBA32F,
                                          buf_width,
                                          buf_height,
                                          GL_RGBA,
                                          GL_UNSIGNED_BYTE);

  uint framebuffer = make_framebuffer(&texture_framebuffer, 1);

  //////////////////////////////////////////////////////////////////////
  // shaders
  //////////////////////////////////////////////////////////////////////

  uint program_test = compile_shader(src_shader_test_vs_glsl_start,
                                     src_shader_test_vs_glsl_size,
                                     src_shader_test_fs_glsl_start,
                                     src_shader_test_fs_glsl_size);
  uint program_test__tex_sampler = glGetUniformLocation(program_test, "tex_sampler");
  uint program_test__tex_size = glGetUniformLocation(program_test, "tex_size");

  //////////////////////////////////////////////////////////////////////
  // buffers
  //////////////////////////////////////////////////////////////////////

  uint vertex_buffer = make_buffer(GL_ARRAY_BUFFER,
                                   triangle_array,
                                   (sizeof (triangle_array)));

  uint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        (sizeof (float)) * 2,
                        (void*)0
                        );
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  while(!glfwWindowShouldClose(window)) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    //

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, buf_width, buf_height);

    glClearColor(0.78, 0.56, 0.34, 0.12);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_input);
    uint draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);
    glUseProgram(program_test);
    glUniform1i(program_test__tex_sampler, 0);
    glUniform4f(program_test__tex_size,
                input_width, input_height,
                0.5f / input_width,
                0.5f / input_height);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    float out[4 * buf_width * buf_height] = {};
    glReadPixels(0, 0,
                 buf_width, buf_height,
                 GL_RGBA,
                 GL_FLOAT,
                 out);

    for (int y = 0; y < buf_height; y++) {
      for (int x = 0; x < buf_width; x++) {
        printf("[% 2.3f % 2.3f % 2.3f % 2.3f ] ",
               out[(y * buf_width + x) * 4 + 0],
               out[(y * buf_width + x) * 4 + 1],
               out[(y * buf_width + x) * 4 + 2],
               out[(y * buf_width + x) * 4 + 3]);
      }
      printf("\n");
    }
    int sum = 0;
    for (int i = 0; i < buf_width * buf_height; i++) {
      printf("%d ", data[i]);
      if ((i % 4) == 3)
        printf("\n");
      sum += data[i];
    }
    printf("sum: %d\n", sum);
    break;

    /*
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, vp_width, vp_height);

    glClearColor(0.1, 0.2, 0.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_test);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    */

    //glFlush();
    //glFinish();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
