// Own Headers
#include "glitter.hpp"
#include "shader.h"

// 3rd party headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// STL Headers
#include <cstdio>
#include <cstdlib>

int main(int argc, char * argv[]) {

  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

  // Check for Valid Context
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  // Set up shaders.
  Shader leftTriangleShader("leftTriangle.vert", "leftTriangle.frag");
  Shader rightTriangleShader("rightTriangle.vert", "rightTriangle.frag");

  // Set up vertex data and buffers, and configure vertex attributes.
  float verticesLeftTriangle[] = {
      // position          // color
      -1.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom left
       0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom middle
      -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top left
  };
  float verticesRightTriangle[] = {
      0.0f, -0.5f, 0.0f,  // bottom middle
      1.0f, -0.5f, 0.0f,  // bottom right
      0.5f,  0.5f, 0.0f   // top right
  };

  // Vertex buffer object and vertex array object.
  unsigned int VBO[2], VAO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);
  //glGenBuffers(1, &EBO);
  // We first bind the vertex array object for the left triangle.
  glBindVertexArray(VAO[0]);

  // Bind Vertex array buffer.
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeftTriangle), verticesLeftTriangle, GL_STATIC_DRAW);

  // Bind element array buffer object.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Configure data layout of the left triangle.
  // 6 float values per vertex.
  // - first 3 values represent position
  // - second 3 values represent color
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Bind the vertex array object for the right triangle.
  glBindVertexArray(VAO[1]);

  // Bind Vertex array buffer.
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRightTriangle), verticesRightTriangle, GL_STATIC_DRAW);

  // Configure data layout of the current bound vertex array object.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw left triangle.
    leftTriangleShader.use();
    glBindVertexArray(VAO[0]);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw right triangle.
    rightTriangleShader.use();
    // Change the color of the right triangle through a uniform variable.
    auto timeValue = static_cast<float>(glfwGetTime());
    auto greenValue = (sinf(timeValue) / 2.f) + 0.5f;
    // Note: We need to make use of the shader program before setting the color
    //       through n uniform variable.
    rightTriangleShader.setVec4f("outColor", {0.f, greenValue, 0.f, 1.f});
    glBindVertexArray(VAO[1]);
    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindVertexArray(0);
    // glBindVertexArray(0);  // No need to unbind it every time, as we only
    // have a single vertex array object.

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  // De-allocate all resources once they've outlived their purpose.
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);

  glfwTerminate();
  return EXIT_SUCCESS;
}
