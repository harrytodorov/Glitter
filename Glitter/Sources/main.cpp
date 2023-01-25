// Own Headers
#include "glitter.hpp"
#include "shader.h"

// 3rd party headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

// STL Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

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

  // Build and compile shader programs.
  Shader rectangleShader("rectangle.vert", "rectangle.frag");

  // Set up vertex data and buffers, and configure vertex attributes.
  float verticesRectangle[] = {
      // positions          // colors           // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
  };
  unsigned int rectangleIndices[] = {
      0, 1, 3,  // left triangle
      1, 2, 3  // right triangle
  };

  // Vertex buffer object and vertex array object.
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // We first bind the vertex array object for the left triangle.
  glBindVertexArray(VAO);

  // Bind Vertex array buffer.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRectangle), verticesRectangle, GL_STATIC_DRAW);

  // Create the element buffer object.
  GLuint EBO;
  glGenBuffers(1, &EBO);

  // Bind it and configure the data layout.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

  // Configure data layout for the rectangle.
  // Vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Set up the container texture.
  GLuint containerTexture;
  glGenTextures(1, &containerTexture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, containerTexture);
  // Configure the texture warping/filtering options on the
  // currently bound texture object.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load the texture image.
  int widthContainer, heightContainer, nrChannelsContainer;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *textureDataContainer = stbi_load("container.jpg",
                                         &widthContainer, &heightContainer, &nrChannelsContainer, 0);
  if (textureDataContainer)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthContainer, heightContainer, 0, GL_RGB, GL_UNSIGNED_BYTE, textureDataContainer);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture." << std::endl;
  }

  // Free the texture image memory.
  stbi_image_free(textureDataContainer);

  // Set up the face texture.
  GLuint faceTexture;
  glGenTextures(1, &faceTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, faceTexture);
  // Configure the texture warping/filtering options on the
  // currently bound texture object.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load the texture image.
  int widthFace, heightFace, nrChannelsFace;
  unsigned char *textureDataFace = stbi_load("awesomeface.png",
                                              &widthFace, &heightFace, &nrChannelsFace, 0);
  if (textureDataFace)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthFace, heightFace, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureDataFace);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture." << std::endl;
  }

  // Free the texture image memory.
  stbi_image_free(textureDataFace);

  rectangleShader.use();
  rectangleShader.setInt("containerTexture", 0);
  rectangleShader.setInt("faceTexture", 1);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    rectangleShader.use();
    // Switch between active textures.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, faceTexture);

    // Draw rectangle.
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  // De-allocate all resources once they've outlived their purpose.
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return EXIT_SUCCESS;
}
