#pragma once
#include <stdinc.h>
#include "Shader.h"
using namespace std;

namespace gfx {

class Polygon {
public:
  Polygon(unsigned int dim);
  Polygon(unsigned int dim, vector<float> vertices);
  ~Polygon();

  vector<float> vertexes;
  unsigned int dimensions;

  GLuint vao;
  GLuint vbo;

  shared_ptr<Shader> shader;
  // Shader shader;
  GLint drawType = GL_STATIC_DRAW;

  void draw();

private:
  void init();
};

}