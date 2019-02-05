#include "Polygon.h"

Polygon::Polygon(uint dim) {
  dimensions = dim;
  init();
}
Polygon::Polygon(uint dim, vector<float> verts) {
  dimensions = dim;
  vertices = verts;
  init();
}

Polygon::~Polygon() {}

void Polygon::init() {
  D("polygon init")
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
}

void Polygon::draw() {
  D("polygon draw")
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], drawType);
  shader->use();
  shader->commitVertexAttribute("position", 2);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}