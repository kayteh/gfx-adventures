#pragma once
#include <stdinc.h>
#include "Geom.h"
using namespace std;

class App {
public:
  App();
  ~App();

  GLFWwindow *window;

  void mainLoop();
  void earlyUpdate();
  void update();
  void lateUpdate();
  void fixedUpdate();

  void init();
  static void reloadShaders();

  void createEntities();

private:
  vector<shared_ptr<Geom>> entities;
};