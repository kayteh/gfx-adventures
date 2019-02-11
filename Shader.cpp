#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
namespace fs = std::filesystem;

// OpenGL Shader Class Implementation
int typeFromExt(string path) {
  if (path.find(".vert") != string::npos) {
    return GL_VERTEX_SHADER;
  } else if (path.find(".frag") != string::npos) {
    return GL_FRAGMENT_SHADER;
  } else if (path.find(".geom") != string::npos) {
    return GL_GEOMETRY_SHADER;
  } else {
    cerr << "SHADER LOAD ERROR: file " << path.c_str()
         << " wasn't marked in a way we'd know what it is." << endl;
    exit(101);
  }
}

bool checkStatusPart(GLuint shaderPart) {
  GLint status;
  char statusBuffer[512];
  glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &status);
  glGetShaderInfoLog(shaderPart, 512, NULL, statusBuffer);

#ifdef NDEBUG
  if (status != GL_TRUE) {
#endif
#if defined(NDEBUG) || defined(DEBUG_SHADERS)
    cout << "Shader Compile Log:" << endl;
    cout << statusBuffer << endl << endl;
#endif
#ifdef NDEBUG
  }
#endif

  if (status != GL_TRUE) {
    return false;
  }

  return true;
}

Shader::Shader() {
  DS("shader constructor")
  reset();
}

Shader::~Shader() {
  DS("shader deconstructor")
  glDeleteProgram(program);
}

void Shader::loadFiles(vector<string> files) {
  DS("shader load files")
  for (auto const &file : files) {
    loadFile(file);
  }
}

void Shader::loadFile(string file) {
  DS("shader load file")
  ifstream fileStream(file);
  stringstream code;
  if (fileStream.is_open()) {
    code << fileStream.rdbuf();
#ifdef DEBUG_SHADERS
    cout << "SHADER FILE: " << file.c_str() << endl;
#endif // DEBUG
    loadString(code.str(), typeFromExt(file));
  }
}

void Shader::loadFileCombined(string file) {
  DS("shader load combined file")
  ifstream fileStream(file);
  stringstream code;
  if (fileStream.is_open()) {
    code << fileStream.rdbuf();
    map<int, string> splitShaders = preprocessGLSLPragma(code.str());

    for (const auto &c : splitShaders) {
      loadString(c.second, c.first);
    }
  }
}

void Shader::loadString(string shaderCode, int type) {
  DS("shader load string")

  preprocessGLSLIncludes(&shaderCode);

  GLuint part = glCreateShader(type);
  const char *code = shaderCode.c_str();
  glShaderSource(part, 1, &code, NULL);
  glCompileShader(part);
  checkStatusPart(part);

  // #ifdef DEBUG
  // cout << "SHADER CODE: ---" << endl << code << endl;
  // #endif // DEBUG

  // commit and delete
  glAttachShader(program, part);
  glDeleteShader(part);
}

void Shader::link() {
  DS("shader link")
  glLinkProgram(program);
  uniforms.clear();
  linked = true;
}

void Shader::use() {
  // DS("shader use")
  if (linked == false) {
    link();
  }

  glUseProgram(program);
}

void Shader::reset() {
  D("shader reset")
  glDeleteProgram(program);
  program = glCreateProgram();
  linked = false;
}

static regex glslPragmaFV("\\#pragma (\\bfragment|vertex\\b) ([a-zA-Z0-9_]+)");
static regex glslPragmaVER("\\#pragma glsl ([0-9]+ ?[a-z]*)");
map<int, string> Shader::preprocessGLSLPragma(string code) {
  DS("preprocess #pragma")
  smatch directive;

  map<int, string> precompiledShaders; // GLint mapped to a shader string.

  vector<string> toRemove; // matched strings that we need to remove afterwards.
                           // technically speaking, we remove it after because
                           // of the moving target it becomes.

  string version = "#version ";
  if (regex_search(code, directive, glslPragmaVER)) {
    version.append(directive.str(1));
  } else {
    version.append(DEFAULT_GLSL_VERSION);
  }

  while (regex_search(code, directive, glslPragmaFV)) {
    stringstream buffer;
    string arg1 = directive.str(1); // the type
    string arg2 = directive.str(2); // the function name
    bool ok = false;
    if (arg1.compare("vertex") == 0) {
      buffer << version << endl;
      buffer << code << endl;
      buffer << "// -- Generated Vertex" << endl;
      buffer << "void main() {" << endl
             << "  " << arg2 << "(gl_Position);" << endl
             << "}";

      precompiledShaders.insert_or_assign(GL_VERTEX_SHADER, buffer.str());
      ok = true;
    } else if (arg1.compare("fragment") == 0) {
      buffer << version << endl;
      buffer << "out vec4 OUT_COLOR;" << endl;
      buffer << code << endl;
      buffer << "// -- Generated Fragment" << endl;
      buffer << "void main() {" << endl
             << "  OUT_COLOR = " << arg2 << "();" << endl
             << "}";

      precompiledShaders.insert_or_assign(GL_FRAGMENT_SHADER, buffer.str());
      ok = true;
    }

    if (ok) {
      toRemove.push_back(directive.str(0));
    }

    // and destroy the evidence.
    code.erase(directive.position(), directive.length());

#ifdef DEBUG_SHADERS
    cout << "=== *** REMAINING SEARCH *** ===" << endl
         << code << endl
         << "=== *** END REMAINING SEARCH ***" << endl;
#endif
  }

  for (const auto &psc : precompiledShaders) {
    string sh = psc.second;

    for (const auto &rem : toRemove) {
      size_t pos = sh.find(rem);

      if (pos == string::npos) { // don't do stuff if it's already gone.
        continue;
      }

      sh.erase(pos, rem.length());
    }

    precompiledShaders[psc.first] = sh;

#ifdef DEBUG_SHADERS
    cout << "=== *** PRECOMPILED (" << psc.first << ") *** ===" << endl
         << sh << "=== *** END PRECOMPILED (" << psc.first << ") ***" << endl;
#endif
  }

  return precompiledShaders;
}

static map<string, string> includeCache;
static regex glslInclude("\\#include \"(.*)\"");
void Shader::preprocessGLSLIncludes(string *code) {
  DS("preprocess #include")
  // find includes
  smatch directive;

  while (regex_search(*code, directive, glslInclude)) {
    stringstream injectedCode;

    string fileName = directive.str(1);

    injectedCode << endl << "// INCLUDED FROM: " << fileName << endl;
    if (includeCache.count(fileName) > 0) {
      injectedCode << includeCache[fileName];
    } else {
      stringstream loadedCode;
      auto pathname = fs::absolute(SHADER_PATH "/./" + fileName);
      ifstream data;
      data.open(pathname);
      if (!data.is_open()) {
        pathname = fs::absolute(SHADER_INCLUDES "/./" + fileName);
        data.open(pathname);
        if (!data.is_open()) {
          cout << "WARNING: Shader include " << fileName << " wasn't found."
               << endl;
          includeCache.insert_or_assign(fileName, "");
          continue;
        }
      }
      loadedCode << data.rdbuf();
      includeCache.insert_or_assign(fileName, loadedCode.str());
      injectedCode << loadedCode.rdbuf();
    }
    injectedCode << endl << "// -- END " << fileName << endl;

    code->replace(directive.position(), directive.length(),
                  injectedCode.str().c_str());
  }
}

GLint Shader::uniform(string uniformName) {
  if (uniforms.count(uniformName) > 0) {
    return uniforms[uniformName];
  } else {
    const char *n = uniformName.c_str();
    GLint u = glGetUniformLocation(program, n);
    uniforms.insert_or_assign(uniformName, u);
    return u;
  }
}

void Shader::updateUniformTime(map<string, glm::vec4> times) {
  for (const auto &t : times) {
    GLint i = uniform(t.first);
    glUniform4fv(i, 1, glm::value_ptr(t.second));
  }
}

void Shader::updateAllUniformTimes(
    chrono::time_point<chrono::high_resolution_clock> start,
    chrono::time_point<chrono::high_resolution_clock> cur) {
  float t = chrono::duration_cast<chrono::milliseconds>(start - cur).count();
  glm::vec4 u_Time(t / 20, t, t * 2, t * 3);
  map<string, glm::vec4> times = {{"u_Time", u_Time},
                                  {"u_SinTime", glm::sin(u_Time)},
                                  {"u_CosTime", glm::cos(u_Time)}};

  for (const auto &s : shaders) {
    s.second->updateUniformTime(times);
  }
}

void Shader::set(string uniformName, glm::vec4 v4) {
  GLint u = uniform(uniformName);
  glUniform4fv(u, 1, glm::value_ptr(v4));
}

void Shader::set(string uniformName, glm::vec3 v3) {
  GLint u = uniform(uniformName);
  glUniform3fv(u, 1, glm::value_ptr(v3));
}

void Shader::set(string uniformName, float f) {
  GLint u = uniform(uniformName);
  glUniform1f(u, f);
}