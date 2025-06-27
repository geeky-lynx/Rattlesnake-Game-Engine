#include "shader-compiler.hpp"
#include <iostream>
#include <fstream>
#include <vector>

// TODO: Find better data structure for storing contents of GLSL source files
// TODO: Improve copying content for size
std::optional<uint32_t> Shader::compile_sourcefile(const std::string &filename, GLenum shader_type) {
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
  // std::vector<char> source(250); // reserve 250 bytes
  std::string source;
  while (getline(sourcefile, line)) {
    // std::cout << line << std::endl;
    // for (char character : line)
      // source.push_back(character);
    source.append(line);
    source.push_back('\n');
  }
  // source.push_back('\0');

  // std::cout << "character by character" << std::endl;
  // for (auto c : source)
  //   std::cout << c;
  // std::cout << std::endl;

  // std::cout << "src_raw (" << (source.data() != nullptr) << ')' << std::endl;
  // const char *src_raw = source.data();
  // std::cout << *src_raw << std::endl;
  // const char* src_raw = &source.at(0);
  // const char* src_raw = &source[0];
  // std::cout << "\tdis: " << src_raw << std::endl << *src_raw <<std::endl;
  const char* src_raw = source.c_str();
  glShaderSource(id, 1, &src_raw, nullptr);
  glCompileShader(id);

  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char message[512];
    glGetShaderInfoLog(id, 512, nullptr, message);
    std::cout << "\x1b[31m[ERROR]: Shader compilation failed.\x1b[37m"
      << std::endl << "\tFile name: " << filename << std::endl
      << std::endl << "\tReason:" << std::endl << message << std::endl;
    return std::nullopt;
  }

  
  source.clear();
  sourcefile.close();
  return std::optional(id);
}
