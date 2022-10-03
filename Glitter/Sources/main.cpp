// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Shaders.
const char *vertexShaderSourceL = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "layout (location = 1) in vec3 aColor;\n"
                                  "out vec3 ourColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos, 1.0);\n"
                                  "   ourColor = aColor;\n"
                                  "}\0";
const char *vertexShaderSourceR = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSourceL = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "in vec3 ourColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(ourColor, 1.0);\n"
                                    "}\0";
const char *fragmentShaderSourceR = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "uniform vec4 outColor;"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = outColor;\n"
                                    "}\0";

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

  // Build and compile shader program.
  // ---
  // Vertex shader left triangle.
  unsigned int vertexShaderLeftTriangle;
  vertexShaderLeftTriangle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderLeftTriangle, 1, &vertexShaderSourceL, NULL);
  glCompileShader(vertexShaderLeftTriangle);
  // Check compilation.
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShaderLeftTriangle, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertexShaderLeftTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::LEFT_TRIANGLE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Vertex shader right triangle.
  unsigned int vertexShaderRightTriangle;
  vertexShaderRightTriangle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderRightTriangle, 1, &vertexShaderSourceR, NULL);
  glCompileShader(vertexShaderRightTriangle);
  // Check compilation.
  glGetShaderiv(vertexShaderRightTriangle, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertexShaderRightTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::RIGHT_TRIANGLE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment shader left triangle.
  unsigned int fragmentShaderLeftTriangle;
  fragmentShaderLeftTriangle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderLeftTriangle, 1, &fragmentShaderSourceL, NULL);
  glCompileShader(fragmentShaderLeftTriangle);
  // Check compilation.
  glGetShaderiv(fragmentShaderLeftTriangle, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragmentShaderLeftTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::LEFT_TRIANGLE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Shader program left triangle.
  unsigned int shaderProgramLeftTriangle;
  shaderProgramLeftTriangle = glCreateProgram();
  // Attach and link the shaders.
  glAttachShader(shaderProgramLeftTriangle, vertexShaderLeftTriangle);
  glAttachShader(shaderProgramLeftTriangle, fragmentShaderLeftTriangle);
  glLinkProgram(shaderProgramLeftTriangle);
  // Check linking.
  glGetProgramiv(shaderProgramLeftTriangle, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderProgramLeftTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LEFT_TRIANGLE::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Fragment shader right triangle.
  unsigned int fragmentShaderRightTriangle;
  fragmentShaderRightTriangle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderRightTriangle, 1, &fragmentShaderSourceR, NULL);
  glCompileShader(fragmentShaderRightTriangle);
  // Check compilation.
  glGetShaderiv(fragmentShaderRightTriangle, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragmentShaderRightTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::RIGHT_TRIANGLE::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Shader program right triangle.
  unsigned int shaderProgramRightTriangle;
  shaderProgramRightTriangle = glCreateProgram();
  // Attach and link the shaders.
  glAttachShader(shaderProgramRightTriangle, vertexShaderRightTriangle);
  glAttachShader(shaderProgramRightTriangle, fragmentShaderRightTriangle);
  glLinkProgram(shaderProgramRightTriangle);
  // Check linking.
  glGetProgramiv(shaderProgramRightTriangle, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderProgramRightTriangle, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::RIGHT_TRIANGLE::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Delete the shaders.
  glDeleteShader(vertexShaderLeftTriangle);
  glDeleteShader(vertexShaderRightTriangle);
  glDeleteShader(fragmentShaderLeftTriangle);
  glDeleteShader(fragmentShaderRightTriangle);
  // ---

  // Set up vertex data and buffers, and configure vertex attributes.
  // ---
  float verticesOld[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f, -0.5f, 0.0f, // right
      0.0f,  0.5f, 0.0f  // top
  };
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
  //unsigned int indicesLeftTriangle[] =
  //{
  //    0, 1, 3,  // left triangle
  //    1, 2, 4   // right triangle
  //};

  // Vertex buffer object and vertex array object.
  unsigned int VBO[2], EBO, VAO[2];
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
    glUseProgram(shaderProgramLeftTriangle);
    glBindVertexArray(VAO[0]);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw right triangle.
    glUseProgram(shaderProgramRightTriangle);
    // Change the color of the right triangle through a uniform variable.
    float timeValue = glfwGetTime();
    float greenValue = (sinf(timeValue) / 2.f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgramRightTriangle, "outColor");
    // Note: We need to make use of the shader program before setting the color
    //       through n uniform variable.
    glUniform4f(vertexColorLocation, 0.f, greenValue, 0.f, 1.f);
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
  glDeleteProgram(shaderProgramLeftTriangle);
  glDeleteProgram(shaderProgramRightTriangle);

  glfwTerminate();
  return EXIT_SUCCESS;
}
