#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>

#include "shader-compiler.hpp"

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

  int success;
  char message[512] = {0};
  auto vertex_shader = ShaderCompiler::compile_sourcefile("main.vert.glsl", GL_VERTEX_SHADER);
  uint32_t vertex_shader_id = vertex_shader.value();

  auto fragment_shader = ShaderCompiler::compile_sourcefile("main.frag.glsl", GL_FRAGMENT_SHADER);
  uint32_t fragment_shader_id = fragment_shader.value();

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
