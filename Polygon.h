#pragma once
#include "Shader.h"
#include <stdinc.h>
using namespace std;

class Polygon {
public:
  Polygon(uint dim);
  Polygon(uint dim, vector<float> vertices);
  ~Polygon();

  vector<float> vertexes;
  uint dimensions;

  GLuint vao;
  GLuint vbo;

  shared_ptr<Shader> shader;
  // Shader shader;
  GLint drawType = GL_STATIC_DRAW;

  void draw();

private:
  void init();
};