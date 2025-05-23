#include <cstdint>
#include <optional>
#include <string>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

namespace ShaderCompiler {
  std::optional<uint32_t> compile_sourcefile(const std::string &filename, GLenum shader_type);
}


