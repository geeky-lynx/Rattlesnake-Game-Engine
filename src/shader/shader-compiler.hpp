#include <cstdint>
#include <optional>
#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>

namespace Shader {
  std::optional<uint32_t> compile_sourcefile(const char* filename, GLenum shader_type);
}
