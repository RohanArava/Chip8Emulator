/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_MYAPPLICATION
#define OPENGL_CMAKE_SKELETON_MYAPPLICATION

#include "../GLUtils/Application.hpp"
#include "../GLUtils/Shader.hpp"
class MyApplication : public Application {
 public:
  MyApplication();
  bool setPixel(int x, int y);
  void clear();

 protected:
  virtual void loop();

 private:
  const int size = 100;

  // shader
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;
  bool *display;
  int width;
  int height;
};

#endif  // OPENGL_CMAKE_SKELETON_MYAPPLICATION
