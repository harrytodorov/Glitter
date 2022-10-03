// Own headers
#include "shader.h"

// STL headers
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

Shader::Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath)
{
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  std::string vertexShaderContents;
  std::string fragmentShaderContents;

  // Make sure the shaders can throw exceptions.
  vertexShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  fragmentShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

  // Read in the shaders.
  try {
    // Open files.
    vertexShaderFile.open(vertexShaderPath);
    fragmentShaderFile.open(fragmentShaderPath);

    // Read file's contents into streams.
    std::stringstream vertexShaderFileStream, fragmentShaderFileStream;
    vertexShaderFileStream << vertexShaderFile.rdbuf();
    fragmentShaderFileStream << fragmentShaderFile.rdbuf();

    // Read the whole files into the buffers.
    vertexShaderContents = vertexShaderFileStream.str();
    fragmentShaderContents = fragmentShaderFileStream.str();
  } catch (const std::ifstream::failure& exception)
  {
    std::cerr << "An error occurred while reading in the shader files: " << std::endl;
    std::cerr << "- " << vertexShaderPath << std::endl;
    std::cerr << "- " << fragmentShaderPath << std::endl;
  }

  // Bookkeeping for shader compilation.
  GLint success;
  GLchar errorLogBuffer[512];

  // Create and compile the vertex shader.
  // Note: One needs the const char* of the string to be able to pass it.
  //       See: https://stackoverflow.com/a/38260633/2935386
  const GLchar* vertexShaderContentsCStyle = vertexShaderContents.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderContentsCStyle, nullptr);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(vertexShader, 512, nullptr, errorLogBuffer);
    std::cerr << "An error occurred while compiling vertex shader '"
              << vertexShaderPath << "'." << std::endl;
  }

  // Create and compile fragment shader.
  const GLchar* fragmentShaderContentsCStyle = fragmentShaderContents.c_str();
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderContentsCStyle, nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(fragmentShader, 512, nullptr, errorLogBuffer);
    std::cerr << "An error occurred while compiling fragment shader '"
              << fragmentShaderPath << "'." << std::endl;
  }

  // Create and link the shader program.
  m_ShaderProgramId = glCreateProgram();
  glAttachShader(m_ShaderProgramId, vertexShader);
  glAttachShader(m_ShaderProgramId, fragmentShader);
  glLinkProgram(m_ShaderProgramId);
  glGetProgramiv(m_ShaderProgramId, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(m_ShaderProgramId, 512, nullptr, errorLogBuffer);
    std::cerr << "An error occurred while linking the shader program." << std::endl;
  }

  // Delete the shaders.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() const
{
  glUseProgram(m_ShaderProgramId);
}

void Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_ShaderProgramId, name.c_str()), (GLint)value);
}

void Shader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(m_ShaderProgramId, name.c_str()), (GLint)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), (GLfloat)value);
}

void Shader::setVec4f(const std::string &name, const glm::vec4 &value)
{
  glUniform4f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value.x, value.y, value.z, value.w);
}
