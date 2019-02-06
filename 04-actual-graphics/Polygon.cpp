#include "Polygon.h"

namespace gfx {

Polygon::Polygon(unsigned int dim) {
  dimensions = dim;
  init();
}
Polygon::Polygon(unsigned int dim, vector<float> verts) {
  dimensions = dim;
  vertexes = verts;
  init();
}

Polygon::~Polygon() { shader = nullptr; }

void Polygon::init() {
  D("polygon init")
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
}

void Polygon::draw() {
  // D("polygon draw")
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float),
               &vertexes.front(), drawType);
  shader->use();
  GLint a = glGetAttribLocation(shader->program, "position");
  glVertexAttribPointer(a, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(a);
  glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / dimensions);
}

}