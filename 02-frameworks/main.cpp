#include <stdinc.h>
using namespace std;

map<string, GLuint> shaders;

chrono::time_point startTime = cClock.now();

bool loadShaderProgram(string name, vector<string> files) {
  // array of all the parts of this program
  list<GLuint> shaderParts;

  // output log for all shader files.
  stringstream outputLog;

  // reused parts
  GLuint shaderPart;
  GLint status;
  int shaderType;
  string shaderCode;

  for (auto const &file: files) {
    // shaderCode.clear();
    // codeStream.clear();

    if (file.find("vert.glsl") != string::npos) {
      shaderType = GL_VERTEX_SHADER;
    } else if (file.find("frag.glsl") != string::npos) {
      shaderType = GL_FRAGMENT_SHADER;
    } else if (file.find("geom.glsl") != string::npos) {
      shaderType = GL_GEOMETRY_SHADER;
    } else {
      cerr << "SHADER LOAD ERROR: file " << file.c_str() << " wasn't marked in a way we'd know what it is." << endl;
      exit(100);
    }

    
    ifstream fileStream;
    stringstream codeStream;
    fileStream.open(file);
    if (fileStream.is_open()) {
      codeStream << fileStream.rdbuf();
      shaderCode = codeStream.str();
      
      fileStream.close();
    } else {
      cerr << "SHADER LOAD ERROR: file " << file.c_str() << " couldn't open" << endl;
      exit(101);
    }
    
    #ifdef DBG_SHADERS
    cerr << shaderCode.c_str() << endl;
    #endif
    
    shaderPart = glCreateShader(shaderType);
    const char* code = shaderCode.c_str();
    glShaderSource(shaderPart, 1, &code, NULL);
    glCompileShader(shaderPart);


    glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &status);
    char statusBuffer[512];
    glGetShaderInfoLog(shaderPart, 512, NULL, statusBuffer);


    outputLog << "shader: ["<< file.c_str() <<"] ("<< status <<") \n"<<statusBuffer<<"\n--\n";

    shaderCode.clear();
    if (status != GL_TRUE) {
      cerr << "SHADER COMPILE ERROR: file " << file.c_str() << " couldn't compile." << endl;
      break;
    }

    shaderParts.push_back(shaderPart);
  }

  #ifdef DBG_SHADERS
  cerr << outputLog.str();
  #endif

  if (status != GL_TRUE) {
    return false;
  }

  GLuint shaderProgram = glCreateProgram();
  
  for (const auto &part: shaderParts) {
    glAttachShader(shaderProgram, part);
  }

  glLinkProgram(shaderProgram);

  shaders.insert_or_assign(name, shaderProgram);
  return true;
}

void injectTime(GLuint shader) {
  glUseProgram(shader);
  GLint uTime = glGetUniformLocation(shader, "u_Time");
  GLint uSinTime = glGetUniformLocation(shader, "u_SinTime");
  float t = chrono::duration_cast<chrono::milliseconds>(startTime - cClock.now()).count();
  //t/20, t, t*2, t*3
  glUniform4f(uTime, t/20, t, t*2, t*3);
  glUniform4f(uSinTime, sin(t/20), sin(t), sin(t*2), sin(t*3));
  
}

void shaderUniformUpdate() {
  for (const auto& shader: shaders) {
    injectTime(shader.second);
  }
}

void drawTriangle(float offset, string shaderName) {
  float vertices[] = {
      0.0f + offset,  0.5f + offset,  // Vertex 1 (X, Y)
      0.5f + offset,  -0.5f + offset, // Vertex 2 (X, Y)
      -0.5f + offset, -0.5f + offset // Vertex 3 (X, Y)
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo); // Generate 1 buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  
  GLuint shader = shaders[shaderName];
  glUseProgram(shader);

  GLint posAttrib = glGetAttribLocation(shader, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void glfwError(int error, const char* desc) {
  cout << "ERR (" << error << "): "<< desc << endl;
}

void reloadShaders() {
  vector<string> files;
  files.push_back("triangle.frag.glsl");
  files.push_back("triangle.vert.glsl");
  loadShaderProgram("triangle", files);
}

int main() {
  cout << "started" << endl;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwSetErrorCallback(glfwError);
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  cout << "Buffers are go." << endl;

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  reloadShaders();
  
  glfwSwapBuffers(window);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

#ifdef __APPLE__
  int x,y;
  glfwGetWindowPos(window, &x, &y);
  glfwSetWindowPos(window, ++x, y);
#endif

  bool reloadDown = false;

  while (!glfwWindowShouldClose(window)) {
    shaderUniformUpdate();
    
    drawTriangle(0.2f, "triangle");
    drawTriangle(0.1f, "triangle");
    drawTriangle(0.0f, "triangle");
    glfwSwapBuffers(window);


    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);


    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      if (reloadDown == false) {
        reloadShaders();
        reloadDown = true;
        cout << "Reloaded shaders." << endl;
      }
    } else {
      reloadDown = false;
    }

    GLenum err = glGetError();
    if (err != 0) {
      cout << glewGetErrorString(err) << " (" << err << ")" << endl;
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
      glfwSetWindowShouldClose(window, GL_TRUE);      
    }

    setWindowFPS(window);
    
    #ifdef __APPLE__
    fix_render_on_mac(window);
    #endif
  }

  return 0;
}