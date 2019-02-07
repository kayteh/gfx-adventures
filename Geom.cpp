#include "Geom.h"

Geom::Geom(vector<float> verts, vector<unsigned int> elems) {
  vertexes = verts;
  elements = elems;
  init();
}

Geom::~Geom() {}

void Geom::init() {
  D("Geom init")
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
}

void Geom::bufferElements() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int),
               &elements.front(), drawType);
}

void Geom::bufferVertexes() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float),
               &vertexes.front(), drawType);
}

void Geom::bufferVertexPosition() {
  GLint a = glGetAttribLocation(shader->program, "position");
  glVertexAttribPointer(a, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(a);
}

void Geom::draw() {
  glBindVertexArray(vao);
  bufferElements();
  bufferVertexes();

  shader->use();
  bufferVertexPosition();

  callMaterial();
  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
}