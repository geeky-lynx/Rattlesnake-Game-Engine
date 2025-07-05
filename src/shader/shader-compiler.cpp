#include "shader-compiler.hpp"
#include <iostream>
#include <fstream>
#include <string>

static constexpr size_t ERROR_MSG_LEN = 512;
static char error_message[ERROR_MSG_LEN] = {0};

// TODO: Find better data structure for storing contents of GLSL source files
// TODO: Improve copying content for size
std::optional<uint32_t> Shader::compile_sourcefile(const char* filename, GLenum shader_type) {
  uint32_t id = glCreateShader(shader_type);
  if (id == 0) {
    std::cout << "\x1b[31m[ERROR]: Couldn't create new shader object (returned ID of 0).\x1b[37m" << std::endl;
    return std::nullopt;
  }

  std::ifstream sourcefile(filename);
  if (!sourcefile.is_open()) {
    std::cout << "\x1b[31m[ERROR]: There is no source file named " << filename << ".\x1b[37m." << std::endl;
    return std::nullopt;
  }

  std::string line;
  std::string source;
  while (getline(sourcefile, line)) {
    source.append(line);
    source.push_back('\n');
  }

  const char* src_raw = source.c_str();
  glShaderSource(id, 1, &src_raw, nullptr);
  glCompileShader(id);

  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, ERROR_MSG_LEN, nullptr, error_message);
    std::cout << "\x1b[31m[ERROR]: Shader compilation failed.\x1b[37m"
      << std::endl << "\tFile name: " << filename << std::endl
      << std::endl << "\tReason:" << std::endl << error_message << std::endl;
    return std::nullopt;
  }


  source.clear();
  sourcefile.close();
  return std::optional(id);
}
