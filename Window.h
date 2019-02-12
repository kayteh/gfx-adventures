#pragma once
#include <stdinc.h>

enum EWindowMode {
  EWindowMode_Fullscreen,
  EWindowMode_WindowedFullscreen,
  EWindowMode_Windowed,
};

enum EVsyncMode {
  EVsyncMode_Adaptive = -1, // GSYNC/Freesync and related technologoes.
  EVsyncMode_Off,           // Vsync off
  EVsyncMode_On,            // Vsync on
};

class Window {
public:
  unsigned int width = 0;
  unsigned int height = 0;
  GLFWwindow *window;
  GLFWmonitor *monitor;
  // const GLFWvidmode *vidmode;
  EWindowMode mode;
  EVsyncMode vsMode = EVsyncMode_Adaptive;
  std::string title;

  Window(EWindowMode mode) : mode(mode) {};
  Window(EWindowMode mode, unsigned int width, unsigned int height)
      : mode(mode), width(width), height(height) {};
  ~Window(){};

  Window create();

  Window changeMode(EWindowMode mode);
  Window changeResolution(unsigned int width, unsigned int height);
  Window setOpenGLVersion(int major, int minor);
  Window setRefreshRate(int rate);
  Window syncMonitor();
  Window setVsyncMode(EVsyncMode mode);
  Window setTitle(std::string title);

  Window resetWindow();

private:
  static bool _inited;
  bool _windowCreated = false;
  void _resizeHandler();
  void _errorHandler();

  void _createWindowed();
  void _createFullscreen();
  void _createFullscreenWindowed();
};