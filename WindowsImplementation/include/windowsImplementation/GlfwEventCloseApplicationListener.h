#ifndef WINDOWSIMPLEMENTATION_GLFWEVENTCLOSEAPPLICATION_H
#define WINDOWSIMPLEMENTATION_GLFWEVENTCLOSEAPPLICATION_H

#include <GLFW/glfw3.h>
#include <coreEngine/events/EventCloseApplicationListener.h>

namespace cl {
class GlfwEventCloseApplicationListener : public EventCloseApplicationListener {
 public:
  GlfwEventCloseApplicationListener(GLFWwindow &window) {
    this->window = &window;
  }
  void closeApplication() { glfwSetWindowShouldClose(this->window, GL_TRUE); }

 private:
  GLFWwindow *window;
};
}

#endif  // WINDOWSIMPLEMENTATION_GLFWEVENTCLOSEAPPLICATION_H