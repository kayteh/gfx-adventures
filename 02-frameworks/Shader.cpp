#include "App.h"
#include "Shader.h"

// OpenGL Shader Class Implementation
int typeFromExt(string path) {
  if (path.find(".vert") != string::npos) {
      return GL_VERTEX_SHADER;
    } else if (path.find(".frag") != string::npos) {
      return GL_FRAGMENT_SHADER;
    } else if (path.find(".geom") != string::npos) {
      return GL_GEOMETRY_SHADER;
    } else {
      cerr << "SHADER LOAD ERROR: file " << path.c_str() << " wasn't marked in a way we'd know what it is." << endl;
      exit(101);
    }
}

void checkStatusPart(GLuint shaderPart) {
  GLint status;
  char statusBuffer[512];
  glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &status);
  glGetShaderInfoLog(shaderPart, 512, NULL, statusBuffer);

  #ifdef NDEBUG
  if (status != GL_TRUE) {
  #endif
    cout << "Shader Compile Log:" << endl;
    cout << statusBuffer << endl << endl;
  #ifdef NDEBUG
  }
  #endif
}

Shader::Shader() {
  D("shader constructor")
  program = glCreateProgram();
}

Shader::~Shader() {
  D("shader deconstructor")
  glDeleteProgram(program);
  shaders.erase(name);
}

void Shader::loadFiles(vector<string> files) {
  D("shader load files")
  for (auto const& file: files) {
    loadFile(file);
  }
}

void Shader::loadFile(string file) {
  D("shader load file")
  ifstream fileStream (file);
  stringstream code;
  if (fileStream.is_open()) {
    code << fileStream.rdbuf();
    #ifdef DEBUG
    cout << "SHADER FILE: " << file.c_str() << endl;
    #endif // DEBUG
    loadString(code.str(), typeFromExt(file));
  }
}

void Shader::loadString(string shaderCode, int type) {
  D("shader load string")
  GLuint part = glCreateShader(type);
  const char* code = shaderCode.c_str();
  glShaderSource(part, 1, &code, NULL);
  glCompileShader(part);
  checkStatusPart(part);

  // #ifdef DEBUG
  // cout << "SHADER CODE: ---" << endl << code << endl; 
  // #endif // DEBUG

  parts.push_back(part);
}

void Shader::link() {
  D("shader link")
  for (auto const& part: parts) {
    glAttachShader(program, part);
  }

  glLinkProgram(program);
  linked = true;
}

void Shader::use() {
  D("shader use")
  if (linked == false) {
    link();
  }

  glUseProgram(program);
}

void Shader::reset() {
  D("shader reset")
  parts.clear();
  linked = false;
}

void Shader::commitVertexAttribute(const char* attribute, int length) {
  D("shader commit vertex attribute")
  GLint a = glGetAttribLocation(program, attribute);
  glVertexAttribPointer(a, length, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(a);
}