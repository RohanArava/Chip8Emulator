/**
 * MyApplication.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "../GLUtils/glError.hpp"
#include "GLUtils/asset.hpp"

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

float heightMap(const glm::vec2 position) {
  return 2.0 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position) {
  const glm::vec2 dx(1.0, 0.0);
  const glm::vec2 dy(0.0, 1.0);

  VertexType v;
  float h = heightMap(position);

  v.position = glm::vec3(position, 1);
  v.normal = glm::normalize(glm::vec3(0, 0, 1.0));
  v.color = glm::vec4(0.8, 0.8, 0.8, 1.0);
  return v;
}

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}),
      width(64),
      height(32) {
  display = (bool*)malloc(sizeof(bool) * width * height);
  glCheckError(__FILE__, __LINE__);

  // creation of the mesh ------------------------------------------------------
  std::vector<VertexType> vertices;
  std::vector<GLuint> index;
  int pxr = 64.0f;
  int pyr = 32.0f;
  int pxp = 0.0f;
  int pyp = 0.0f;
  vertices.push_back(getHeightMap({-1.0f, 1.0f}));
  vertices.push_back(getHeightMap({-1.0f, (1.0f + (-1.0f / 16.0f))}));
  vertices.push_back(getHeightMap({(-1.0f - (-1.0f / 32.0f)), 1.0f}));
  vertices.push_back(
      getHeightMap({(-1.0f - (-1.0f / 32.0f)), (1.0f + (-1.0f / 16.0f))}));

  index.push_back(0);
  index.push_back(1);
  index.push_back(2);
  index.push_back(1);
  index.push_back(2);
  index.push_back(3);
  std::cout << "vertices=" << vertices.size() << std::endl;
  std::cout << "index=" << index.size() << std::endl;

  // creation of the vertex array buffer----------------------------------------

  // vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ibo
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
               index.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // map vbo to shader attributes
  shaderProgram.setAttribute("position", 3, sizeof(VertexType),
                             offsetof(VertexType, position));
  shaderProgram.setAttribute("normal", 3, sizeof(VertexType),
                             offsetof(VertexType, normal));
  shaderProgram.setAttribute("color", 4, sizeof(VertexType),
                             offsetof(VertexType, color));

  // bind the ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  // vao end
  glBindVertexArray(0);
}

void MyApplication::loop() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  float t = getTime();
  // set matrix : projection + view
  // projection = glm::perspective(float(2.0 * atan(getHeight() / 1920.f)),
  //                               getWindowRatio(), 0.1f, 100.f);
  projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.f);
  view = glm::lookAt(glm::vec3(0.0, 0.0, 20.0), glm::vec3(0.0, 0.0, 0.0),
                     glm::vec3(0.0, 1.0, 0.0));

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  // send uniforms
  shaderProgram.setUniform("projection", projection);
  shaderProgram.setUniform("view", view);

  glCheckError(__FILE__, __LINE__);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glCheckError(__FILE__, __LINE__);
  clear();
  // printf("%d %d\n", (int)(64.0*abs(sin(t*0.001))), (int)(32.0*abs(cos(t*0.001))));
  setPixel((int)(64.0*abs(sin(t*0.1))), (int)(32.0*abs(cos(t*0.1))));
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (display[i + j * width]) {
        shaderProgram.setUniform(
            "pos", glm::vec3(i * ((2.0f / (float)width)), -j * ((2.0f / (float)height)), 0.0f));
        glDrawElements(GL_TRIANGLES,     // mode
                       6,                // count
                       GL_UNSIGNED_INT,  // type
                       NULL              // element array buffer offset
        );
      }
    }
  }

  glBindVertexArray(0);

  shaderProgram.unuse();
}


bool MyApplication::setPixel(int x, int y){
  if(x < 0 || x >= width || y < 0 || y >= height){
    return false;
  }
  display[x + y * width] ^= true;
  // std::cout << x << " " << y << " " << display[x + y * width] << std::endl;
  return !display[x + y * width];
}

void MyApplication::clear(){
  for(int i = 0; i < width * height; i++){
    display[i] = false;
  }
}