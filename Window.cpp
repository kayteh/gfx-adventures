#include <stdinc.h>
#include "Window.h"

bool Window::_inited = false;

/**
 * Creates the window from current data.
 */
Window Window::create() {
  // can't re-create a window.
  if (_windowCreated) {
    std::cerr << "Window was already created." << std::endl;
    exit(10);
  }

  if (!_inited) {
    glfwInit();
    _inited = true;
  }

  syncMonitor();

  // -- preliminary checks!
  if (mode == EWindowMode_Windowed) {
    // height/width check, only in windowed.
    if (width == 0 || height == 0) {
      std::cerr << "Width or Height not set, but it needed to be. Fatal." << std::endl;
      exit(11);
    }
  } else {
    // sync height/widths
    auto vidmode = glfwGetVideoMode(monitor);
    width = vidmode->width;
    height = vidmode->height;
  }

  // -- beginning of the hints
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  // -- window creation time~
  switch(mode) {
    case EWindowMode_Windowed:
      _createWindowed();
      break;
    case EWindowMode_Fullscreen:
      _createFullscreen();
      break;
    case EWindowMode_WindowedFullscreen:
      _createFullscreenWindowed();
      break;
  }

  glfwMakeContextCurrent(window);
  _windowCreated = true;
  
  setVsyncMode(vsMode);

  // -- GLEW, we're basically done!
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != 0) {
    std::cout << "Window: GLEW error: " << glewGetErrorString(err) << std::endl;
    exit(12);
  }

  return *this;
}

Window Window::changeMode(EWindowMode _mode) {
  if (mode != _mode) {
    mode = _mode;
    if (_windowCreated) {
      resetWindow();
    }
  }
  return *this;
}

Window Window::changeResolution(unsigned int w, unsigned int h) {
  if (w != width || h != height) {
    width = w;
    height = h;

    if (_windowCreated) {
      glfwSetWindowSize(window, w, h);
    }
  }

  return *this;
}
Window Window::setRefreshRate(int rate) {
  return *this;
}

Window Window::syncMonitor() {
  monitor = glfwGetPrimaryMonitor();
  auto vidmode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

  return *this;
}

Window Window::setVsyncMode(EVsyncMode _mode) {
  if (vsMode != _mode) {

    if (_windowCreated) {
      if (_mode == EVsyncMode_Adaptive) {
        // if Adaptive sync isn't supported, we fallback to on.
        if (!glfwExtensionSupported("WGL_EXT_swap_control_tear") && !glfwExtensionSupported("GLX_EXT_swap_control_tear")) {
          _mode = EVsyncMode_On;
        }
      }

      glfwSwapInterval(_mode);
    }

    vsMode = _mode;
  }

  return *this;
}

Window Window::setTitle(std::string _title) {
  title = _title;

  if (_windowCreated) {
    glfwSetWindowTitle(window, title.c_str());
  }

  return *this;
}

Window Window::resetWindow() {
  _windowCreated = false;
  glfwDestroyWindow(window);

  return *this;
}

void Window::_createWindowed() {
  glfwWindowHint(GLFW_RESIZABLE, 1);
  glfwWindowHint(GLFW_DECORATED, 1);
  glfwWindowHint(GLFW_MAXIMIZED, 0);
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void Window::_createFullscreen() {
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_DECORATED, 0);
  glfwWindowHint(GLFW_MAXIMIZED, 1);
  window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
}

void Window::_createFullscreenWindowed() {
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_MAXIMIZED, 1);
  glfwWindowHint(GLFW_DECORATED, 0);
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}