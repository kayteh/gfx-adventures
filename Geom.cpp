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

void Geom::bufferVertexData(shared_ptr<Shader> shader, vector<float> data) {
  size_t dataSize = data.size() * sizeof(float);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, dataSize,
               &data.front(), drawType);

  GLint pos = glGetAttribLocation(shader->program, "in_Position");
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  // glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(pos);  

  GLint uv = glGetAttribLocation(shader->program, "in_Texcoord0");
  glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3 * sizeof(float) ));
  glEnableVertexAttribArray(uv);
}


void Geom::combineVertexData(vector<float> * data) {
  // what i know is i need to add 2 items for every 3
  data->reserve(vertexes.size() + uvs.size());
  for (int i = 0; i < vertexes.size() / 3; i++) {
    for (int j = 0; j < 3; j++) {
      data->push_back(vertexes[i*3+j]);
    }

    data->push_back(uvs[i*2]);
    data->push_back(uvs[i*2+1]);
    cout.flush();
  }
}

void Geom::draw() {
  glBindVertexArray(vao);
  bufferElements();

  auto shader = Shader::get(shaderName);
  shader->use();

  vector<float> combinedVertexData;
  combineVertexData(&combinedVertexData);

  bufferVertexData(shader, combinedVertexData);

  callMaterial(shader);

  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
}