#include <stdinc.h>
#include <deferrable.h>
#include "App.h"
#include "Shader.h"
#include "Geom.h"

map<string, vector<string>> shaders = {
    {"triangle", {"shaders/triangle.frag", "shaders/triangle.vert"}},
    {"triangle_combined", {"shaders/triangle_combined.glsl"}},
};

chrono::time_point<chrono::high_resolution_clock> startTime;
chrono::time_point<chrono::high_resolution_clock> lastFixedUpdate;
chrono::time_point<chrono::high_resolution_clock> lastDrawTick;
chrono::milliseconds fixedUpdateDelta =
    chrono::milliseconds(FIXED_UPDATE_DELTA);
chrono::milliseconds maxFps = chrono::milliseconds(1000 / MAX_FPS);
bool reloadDown = false;

App::App() {}
App::~App() {
  D("APP DECONST")
  entities.clear();
  // Shader::shaders.clear();
  glfwSetWindowShouldClose(window->window, GL_TRUE);
}

void App::init() {
  D("app - init")

  window = make_unique<Window>(EWindowMode_Windowed, 800, 600);
  window->create();

  // setup buffers and GL things
  glGenBuffers(1, &vertexBuffer);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  // setup shaders, entities
  // auto es = make_shared<Shader>();
  // es->name = "@internal/error";
  // es->loadFileCombined("shader/err.glsl");
  // Shader::errorShader = es;

  deferrable::defer p([] { glClearColor(1.f, 0.f, 0.5f, 1.f); }, 5s);

  reloadShaders();

  createEntities();
}

void App::reloadShaders() {
  D("app - reload shaders")
  for (auto const &s : shaders) {
    shared_ptr<Shader> sh;
    string n = s.first;
    if (Shader::shaders.find(n) != Shader::shaders.end()) {
      D("reload -- found existing")
      sh = Shader::shaders[n];
      sh->reset();
    } else {
      D("reload -- creating new")
      sh = make_shared<Shader>();
      sh->name = n;
    }

    if (s.second.size() == 1) {
      sh->loadFileCombined(s.second[0]);
    } else {
      sh->loadFiles(s.second);
    }
    sh->link();
    Shader::shaders.insert_or_assign(n, sh);
  }
}

void App::createEntities() {
  D("app - create entities")
  for (unsigned int i = 0; i < 3; i++) {
    float o = 0.1 * i;
    auto p = make_shared<Geom>(
        vector<float>{
            -0.5f + o, 0.5f + o, 1.0f + o,  // Top-left
            0.5f + o, 0.5f + o, 0.0f + o,   // Top-right
            0.5f + o, -0.5f + o, 0.0f + o,  // Bottom-right
            -0.5f + o, -0.5f + o, 1.0f + o, // Bottom-left
        },
        vector<unsigned int>{0, 1, 2, 2, 3, 0});

    p->shaderName = "triangle_combined";
    p->setMaterialCallback([o](auto shader) { shader->set("u_Depth", o * 3); });

    entities.push_back(p);
  }
}

void App::mainLoop() {
  D("main loop fired")
  chrono::time_point<chrono::high_resolution_clock> currentClock =
      chrono::high_resolution_clock::now();
  startTime = currentClock;
  lastDrawTick = currentClock;
  lastFixedUpdate = currentClock;

  cout << "TIMINGS: max fps = " << MAX_FPS << " (" << 1000 / MAX_FPS
       << "ms); fixed delta time = " << FIXED_UPDATE_DELTA << "ms" << endl;

  auto fixedUpdateDiff = currentClock - lastFixedUpdate;
  auto drawUpdateDiff = currentClock - lastDrawTick;

  // D("main loop prewarmed")
  while (!glfwWindowShouldClose(window->window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // D("loop")
    currentClock = chrono::high_resolution_clock::now(); //

    // fixed updates
    fixedUpdateDiff = currentClock - lastFixedUpdate;
    if (fixedUpdateDiff >= fixedUpdateDelta) {
      // D("FIXED UPDATE")
      lastFixedUpdate = currentClock;
      setWindowFixedUpdate();
      fixedUpdate();
    }

    // regular draws
    drawUpdateDiff = currentClock - lastDrawTick;
    if (drawUpdateDiff >= maxFps) {
      // D("DRAWING")
      lastDrawTick = currentClock;
      setWindowFPS();

      earlyUpdate();
      update();
      lateUpdate();

      // shims
      fix_render_on_mac(window->window);

    } else {
      std::chrono::duration<float, std::milli> a = maxFps - drawUpdateDiff;
      std::chrono::duration<float, std::milli> b =
          fixedUpdateDelta - fixedUpdateDiff;

      float waitTime = min(a.count(), b.count());

      // if there wasn't a draw, let's tap the thread for a ms to prevent
      this_thread::sleep_for(
          std::chrono::duration<float, std::milli>(waitTime));
    }

    updateWindowTitle(window->window);
  }
}

void App::earlyUpdate() {
  deferrable::tick();
  Shader::updateAllUniformTimes(startTime, lastDrawTick);
}

void App::update() {
  // D("app - update")
  for (auto const &poly : entities) {
    poly->draw();
  }
}

void App::lateUpdate() {
  // D("app - lateUpdate")
  glfwSwapBuffers(window->window);
  glfwPollEvents();
  if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window->window, GL_TRUE);

  if (glfwGetKey(window->window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
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