#include "Geom.h"

Geom::Geom(vector<float> verts, vector<unsigned int> elems, vector<float> uvs)
    : vertexes(verts), elements(elems), uvs(uvs) {
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

void Geom::bufferVertexPosition(shared_ptr<Shader> shader) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float),
               &vertexes.front(), drawType);
  GLint a = glGetAttribLocation(shader->program, "in_Position");
  glVertexAttribPointer(a, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(a);
}

void Geom::bufferVertexUV(shared_ptr<Shader> shader) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float),
               &uvs.front(), drawType);
  GLint a = glGetAttribLocation(shader->program, "in_Texcoord0");
  glVertexAttribPointer(a, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(a);
}

void Geom::draw() {
  glBindVertexArray(vao);
  bufferElements();

  auto shader = Shader::get(shaderName);
  shader->use();

  bufferVertexPosition(shader);
  bufferVertexUV(shader);

  callMaterial(shader);

  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
}