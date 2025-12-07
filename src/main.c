#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include "opengl.h"

typedef unsigned int uint;

int vp_width = 800;
int vp_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  vp_width = width;
  vp_height = height;
}

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

  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  const char * data = "arst";

  make_texture(data,
               GL_R8, // internalformat
               2, // width
               2, // height
               GL_RED,
               GL_UNSIGNED_BYTE);

  uint texture_framebuffer = make_texture(NULL,
                                          GL_RGBA32F,
                                          2,
                                          2,
                                          GL_RGBA,
                                          GL_UNSIGNED_BYTE);

  uint fp_framebuffer;
  glGenFramebuffers(1, &fp_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, fp_framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_framebuffer, 0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  assert(status == GL_FRAMEBUFFER_COMPLETE);

  uint draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);

  while(!glfwWindowShouldClose(window)) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    glBindFramebuffer(GL_FRAMEBUFFER, fp_framebuffer);
    glViewport(0, 0, 2, 2);

    glClearColor(0.1, 0.2, 0.3, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    //glFlush();
    //glFinish();

    float out[4 * 2 * 2] = {0};
    glReadPixels(0, 0,
                 2, 2,
                 GL_RGBA,
                 GL_FLOAT,
                 out);

    for (int i = 0; i < 4 * 2 * 2; i++) {
      printf("%f\n", out[i]);
    }

    break;
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
