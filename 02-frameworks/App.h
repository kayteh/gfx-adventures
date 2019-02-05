#pragma once
#include <stdinc.h>
#include "Polygon.h"
#include "Shader.h"
using namespace std;

class App {
public:
  App();
  ~App();

  GLFWwindow* window;

  void mainLoop();
  void earlyUpdate();
  void update();
  void lateUpdate();
  void fixedUpdate();

  void init();
  void reloadShaders();

  void createEntities();

private:
  vector<shared_ptr<Polygon>> entities;

  friend class Shader;
  friend class Polygon;
};