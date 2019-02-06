#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <thread>
#include <vector>

#ifdef DEBUG
#define D(text) { std::cout << "DEBUG: " << text << endl; };
#else
#define D(t) {};
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE "GFX Adventures"
#endif 

static std::chrono::high_resolution_clock cClock;
static uint64_t ulFrameCounter = 0;
static std::chrono::time_point tLast = cClock.now();
static std::string sTitle = WINDOW_TITLE;
inline void setWindowFPS(GLFWwindow* window) {
  ulFrameCounter += 1;
  if (cClock.now() - tLast >= std::chrono::seconds(1)) {
    std::stringstream title;
    title << sTitle << " [ FPS: " << ulFrameCounter << " ]";
    glfwSetWindowTitle(window, title.str().c_str());
    ulFrameCounter = 0;
    tLast = cClock.now();
  }
}

static bool macMoved = false;
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