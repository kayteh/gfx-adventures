#pragma once
#include <stdinc.h>
#include "Shader.h"
using namespace std;

class Polygon {
public:
  Polygon(uint dim);
  Polygon(uint dim, vector<float> vertices);
  ~Polygon();

  vector<float> vertices;
  uint dimensions;

  GLuint vao;
  GLuint vbo;

  Shader* shader;
  // Shader shader;
  GLint drawType = GL_STATIC_DRAW;

  void draw();
private:
  void init();
};