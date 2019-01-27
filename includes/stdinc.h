#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <thread>
#include <vector>

inline void fix_render_on_mac(GLFWwindow *window) {
#ifdef __APPLE__
  static bool macMoved = false;
  if (!macMoved) {
    int x, y;
    glfwGetWindowPos(window, &x, &y);
    glfwSetWindowPos(window, ++x, y);
    macMoved = true;
  }
#endif
}

double lastTime = 0;
long nbFrames = 0;

inline void setWindowFPS(GLFWwindow *win) {
  // Measure speed
  double currentTime = glfwGetTime();
  nbFrames++;

  if (currentTime - lastTime >= 1.0) { // If last cout was more than 1 sec ago
    char title[256];
    title[255] = '\0';

    snprintf(title, 255, "%s - [FPS: %3.2f]", "OpenGL",
             1000.0f / (float)nbFrames);

    glfwSetWindowTitle(win, title);

    nbFrames = 0;
    lastTime = currentTime;
  }
}