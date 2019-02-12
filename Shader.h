// See shaders/triangle_combined.shader for example
// of how this loader works.
//
// I took a lot of inspiration from Unity but
// I think I took a smarter path in code generation.

#pragma once
#include <stdinc.h>
using namespace std;

#ifdef DEBUG_SHADERS
#define DS(text)                                                               \
  { std::cout << "DEBUG: " << text << std::endl; };
#else
#define DS(t) {};
#endif

class Shader {
public:
  Shader();
  ~Shader();

  GLuint program;
  string name;

  void loadFiles(vector<string> files);
  void loadFile(string file);
  void loadFileCombined(string file);
  void loadString(string shaderCode, int type);
  void link();
  void use();
  void reset();
  void set(string uniformName, glm::vec4 v4);
  void set(string uniformName, glm::vec3 v3);
  void set(string uniformName, float f);

  GLint uniform(string uniformName);

  void updateUniformTime(map<string, glm::vec4> times);

  inline static shared_ptr<Shader> get(string name) { return shaders[name]; }

  inline static map<string, shared_ptr<Shader>> shaders;
  bool linked = false;

  map<int, string> preprocessGLSLPragma(string code);
  void preprocessGLSLIncludes(string *code);
  static void
  updateAllUniformTimes(chrono::time_point<chrono::high_resolution_clock> start,
                        chrono::time_point<chrono::high_resolution_clock> cur);

private:
  map<string, GLint> uniforms;
};
