#pragma once
#include <stdinc.h>
#include "Geom.h"
#include "Window.h"
using namespace std;

#ifndef FIXED_UPDATE_DELTA
#define FIXED_UPDATE_DELTA 16
#endif

#ifndef MAX_FPS
#define MAX_FPS 60
#endif

class App {
public:
  unique_ptr<Window> window;

  chrono::time_point<chrono::high_resolution_clock> startTime;
  chrono::time_point<chrono::high_resolution_clock> lastFixedUpdate;
  chrono::time_point<chrono::high_resolution_clock> lastDrawTick;
  chrono::milliseconds fixedUpdateDelta;
  chrono::milliseconds maxFps;
  GLuint vertexBuffer;

  App();
  ~App();

  void init();
  static void reloadShaders();
  void createEntities();

  void mainLoop();

  void earlyUpdate();
  void update();
  void lateUpdate();
  void fixedUpdate();

private:
  vector<shared_ptr<Geom>> entities;
};