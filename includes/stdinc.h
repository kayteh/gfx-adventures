#pragma once
#ifdef __WIN32__
#define NOGDI
#include <Windows.h>
#endif

#define GLM_FORCE_CXX17
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>
#include <filesystem>
// #include <experimental/filesystem>
namespace fs = std::filesystem;

#ifdef DEBUG
#define D(text)                                                                \
  { std::cout << "DEBUG: " << text << std::endl; };
#else
#define D(t) {};
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE "GFX Adventures"
#endif

#define SHADER_PATH "./shaders"
#define SHADER_INCLUDES "./includes/"

#ifndef DEFAULT_GLSL_VERSION
#define DEFAULT_GLSL_VERSION "410 core"
#endif

static std::chrono::high_resolution_clock cClock;
static uint64_t ulFrameCounter = 0;
static uint64_t ulFixedUpdateCounter = 0;
static std::chrono::time_point<std::chrono::high_resolution_clock> tLast =
    cClock.now();
static std::string sTitle = WINDOW_TITLE;
inline void updateWindowTitle(GLFWwindow *window) {
  if (cClock.now() - tLast >= std::chrono::seconds(1)) {
    std::stringstream title;
    title << sTitle << " [ FPS: " << ulFrameCounter
          << " ] [ FUT: " << ulFixedUpdateCounter << " ]";
    glfwSetWindowTitle(window, title.str().c_str());
    ulFrameCounter = 0;
    ulFixedUpdateCounter = 0;
    tLast = cClock.now();
  }
}
inline void setWindowFixedUpdate() { ulFixedUpdateCounter += 1; }
inline void setWindowFPS() { ulFrameCounter += 1; }

#ifdef __APPLE__
static bool macMoved = false;
inline void fix_render_on_mac(GLFWwindow *window) {
  if (!macMoved) {
    int x, y;
    glfwGetWindowPos(window, &x, &y);
    glfwSetWindowPos(window, ++x, y);
    macMoved = true;
  }
}
#else
#define fix_render_on_mac(w)                                                   \
  {}
#endif