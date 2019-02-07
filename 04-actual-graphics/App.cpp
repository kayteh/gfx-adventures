#include <stdinc.h>
#include "App.h"
#include "Shader.h"
#include "Geom.h"

#ifndef FIXED_UPDATE_DELTA
#define FIXED_UPDATE_DELTA 16
#endif

#ifndef MAX_FPS
#define MAX_FPS 120
#endif

map<string, vector<string>> shaders = {
    {"triangle", {"shaders/triangle.frag", "shaders/triangle.vert"}},
};

App::App() {}
App::~App() {
  D("APP DECONST")
  entities.clear();
  Shader::shaders.clear();
  glfwSetWindowShouldClose(window, GL_TRUE);
}

// chrono::steady_clock steadyClock;
// chrono::high_resolution_clock hrClock;
static chrono::time_point<chrono::high_resolution_clock> startTime;
static chrono::time_point<chrono::high_resolution_clock> lastFixedUpdate;
static chrono::time_point<chrono::high_resolution_clock> lastDrawTick;

void App::mainLoop() {
  D("main loop fired")
  auto currentClock = chrono::high_resolution_clock::now();
  startTime = currentClock;
  lastDrawTick = currentClock;
  lastFixedUpdate = currentClock;
  // this_thread::sleep_for(1s);

  cout << "TIMINGS: max fps = " << MAX_FPS << " (" << 1000 / MAX_FPS
       << "ms); fixed delta time = " << FIXED_UPDATE_DELTA << "ms" << endl;

  // D("main loop prewarmed")
  while (!glfwWindowShouldClose(window)) {
    // D("loop")
    currentClock = chrono::high_resolution_clock::now(); //

    // fixed updates
    if (currentClock - lastFixedUpdate >=
        chrono::milliseconds(FIXED_UPDATE_DELTA)) {
      // D("FIXED UPDATE")
      lastFixedUpdate = currentClock;
      fixedUpdate();
    }

    // regular draws
    if (currentClock - lastDrawTick >= chrono::milliseconds(1000 / MAX_FPS)) {
      // D("DRAWING")
      lastDrawTick = currentClock;
      earlyUpdate();
      update();
      lateUpdate();

      // shims
      setWindowFPS(window);
      fix_render_on_mac(window);
      // GLenum err = glGetError();
      // if (err != 0) {
      //   std::cout << glewGetErrorString(err) << " (" << err << ")" <<
      //   std::endl; glfwSetWindowShouldClose(window, GL_TRUE); exit(50);
      // }
    } else {
      // if there wasn't a draw, let's tap the thread for a ms to prevent
      // runaway this_thread::sleep_for(0.01s);
    }
  }
}

void App::earlyUpdate() {
  // D("app - early update")
  // tick over

  Shader::updateAllUniformTimes(startTime, lastDrawTick);
}

void App::update() {
  // D("app - update")
  for (auto const &poly : entities) {
    poly->draw();
  }
}

bool reloadDown = false;
void App::lateUpdate() {
  // D("app - lateUpdate")
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
}

void App::fixedUpdate() {
  // D("app - fixed update")
}

static GLuint vertexBuffer;
void App::init() {
  D("app - init")
  // create GLFW context
  // utils::createGLContext(window, 800, 600);
  D("createGLContext")
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwSetErrorCallback(utils::glfwError);
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  window = glfwCreateWindow(800, 600, WINDOW_TITLE, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(81);
  }
  glfwMakeContextCurrent(window);
  D("createGLContext finished")


  glfwSwapInterval(0);

  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // setup GLEW
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != 0) {
    cout << "glew error " << glewGetErrorString(err) << endl;
  }

  // setup buffers and GL things
  glGenBuffers(1, &vertexBuffer);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  // setup shaders, entities
  this_thread::sleep_for(0.5s);
  reloadShaders();

  createEntities();
}

// shared_ptr<Shader> triangle;
void App::reloadShaders() {
  D("app - reload shaders")
  for (auto const &s : shaders) {
    shared_ptr<Shader> sh;
    if (Shader::shaders.count(s.first) > 0) {
      sh = Shader::shaders[s.first];
    } else {
      sh = make_shared<Shader>();
      sh->name = s.first;
      Shader::shaders.insert_or_assign(s.first, shared_ptr<Shader>(sh));
    }
    sh->loadFiles(s.second);
    sh->link();
  }
}

void App::createEntities() {
  D("app - create entities")
  for (unsigned int i = 0; i < 3; i++) {
    float o = 0.1 * i;
    auto p =
        make_shared<Geom>(vector<float>{
          -0.5f + o,  0.5f + o, 1.0f + o, // Top-left
          0.5f + o,  0.5f + o, 0.0f + o, // Top-right
          0.5f + o, -0.5f + o, 0.0f + o, // Bottom-right
          -0.5f + o, -0.5f + o, 1.0f + o, // Bottom-left
        }, vector<unsigned int>{
          0, 1, 2,
          2, 3, 0
        });

    p->shader = Shader::get("triangle");
    p->setMaterialCallback([=](shared_ptr<Shader> shader) {
      shader->set("u_Depth", o*3);
    });
    entities.push_back(shared_ptr<Geom>(p));
  }

}