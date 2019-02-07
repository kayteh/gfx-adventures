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
#define D(text) { std::cout << "DEBUG: " << text << endl; };
#else
#define D(t) {};
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE "GFX Adventures"
#endif 

#define SHADER_PATH "./shaders"
#define SHADER_INCLUDES "./includes/"



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
#define fix_render_on_mac(w) {}
#endif