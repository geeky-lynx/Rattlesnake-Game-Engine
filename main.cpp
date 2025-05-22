#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>

constexpr const char* VERTEX_SHADER_SOURCE =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\n\0"
;

constexpr const char* FRAGMENT_SHADER_SOURCE =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0"
;

void framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_keyboard_inputs(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
    glfwWindowShouldClose(window);
}

int main(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Rattlesnake GE", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create a window." << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

  int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!result) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -2;
  }



  float vertices[] = {
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f
  };

  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  uint32_t vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_id, 1, &VERTEX_SHADER_SOURCE, nullptr);
  glCompileShader(vertex_shader_id);

  int success;
  char message[512] = {0};
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader_id, 512, nullptr, message);
    std::cout << "\x1b[31m[ERROR]: Vertex Shader Compilation failed\x1b[37m" << std::endl
      << message << std::endl;
  }

  uint32_t fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
  glCompileShader(fragment_shader_id);

  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader_id, 512, nullptr, message);
    std::cout << "\x1b[31m[ERROR]: Fragment Shader Compilation failed\x1b[37m" << std::endl
      << message << std::endl;
  }

  uint32_t shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader_id);
  glAttachShader(shader_program, fragment_shader_id);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, nullptr, message);
    std::cout << "\x1b[31m[ERROR]: Shader Linking failed\x1b[37m" << std::endl
      << message << std::endl;
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);




  // Render Loop
  while (!glfwWindowShouldClose(window)) {
    // inputs
    process_keyboard_inputs(window);

    // render
    glClearColor(0.33f, 0.21f, 0.24f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao); // VAO binding again
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  glfwTerminate();
  return 0;
}
