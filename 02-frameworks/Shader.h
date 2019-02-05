#pragma once
#include <stdinc.h>
using namespace std;

class Shader {
public:
  Shader();
  ~Shader();

  GLuint program;
  string name;

  void loadFiles(vector<string> files);
  void loadFile(string file);
  void loadString(string shaderCode, int type);
  void link();
  void use();
  void reset();

  void commitVertexAttribute(const char* attribute, int length);

  inline static shared_ptr<Shader> get(string name) {
    return shaders[name];
  }

  inline static map<string, shared_ptr<Shader>> shaders;
  bool linked = false;

private:
  vector<GLuint> parts;

};
