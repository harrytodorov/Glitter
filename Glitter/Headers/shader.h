#ifndef GLITTER_SHADER_H
#define GLITTER_SHADER_H

// Own headers

// 3rd party headers
#include <glad/glad.h>
#include <glm/vec4.hpp>

// STL headers
#include <string>

class Shader
{
public:
  Shader(const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);
  void use() const;
  // Utility functions for setting up a uniform variable.
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec4f(const std::string& name, const glm::vec4& value);
private:
  // The identifier of the shader program.
  GLuint m_ShaderProgramId;
};

#endif // GLITTER_SHADER_H
