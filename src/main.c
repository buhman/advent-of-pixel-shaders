#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include "opengl.h"

//#include "shader/test.vs.glsl.h"
//#include "shader/test.fs.glsl.h"

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

extern void solution_2025_01(unsigned int vertex_array);

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
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  int max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
  printf("max_texture_size %d\n", max_texture_size);

  while(!glfwWindowShouldClose(window)) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    solution_2025_01(vertex_array);

    break;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
