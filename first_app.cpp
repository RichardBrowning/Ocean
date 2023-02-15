#include "first_app.h"

namespace ocean {
    void FirstApp::run() {
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}