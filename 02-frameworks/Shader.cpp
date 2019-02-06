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

  preprocessGLSL(&shaderCode);

  GLuint part = glCreateShader(type);
  const char* code = shaderCode.c_str();
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
  D("shader link")
  glLinkProgram(program);
  linked = true;
}

void Shader::use() {
  // D("shader use")
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

static map<string, string> includeCache;
static regex glslInclude("\\#include \"(.*)\"");
void Shader::preprocessGLSL(string* code) {
  D("preprocess")
  // find includes
  smatch directive;
  
  while(regex_search(*code, directive, glslInclude)) {
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
          cout << "WARNING: Shader include " << fileName << " wasn't found." << endl;
          includeCache.insert_or_assign(fileName, "");
          continue;
        }
      } 
      loadedCode << data.rdbuf();
      includeCache.insert_or_assign(fileName, loadedCode.str());
      injectedCode << loadedCode.rdbuf();
    }
    injectedCode << endl << "// -- END " << fileName << endl;

    code->replace(
      directive.position(), 
      directive.length(),
      injectedCode.str().c_str()
    );
  }

}