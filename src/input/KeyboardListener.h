#pragma once

#include "OceanGameObject.h"
#include "OceanWindow.h"

namespace ocean{
    class KeyboardListener{
        public:
            struct KeyMap{
                int lookUp = GLFW_KEY_UP;
                int lookDown = GLFW_KEY_DOWN;
                int lookLeft = GLFW_KEY_LEFT;
                int lookRight = GLFW_KEY_RIGHT;
                
                int moveForward = GLFW_KEY_W;
                int moveBackward = GLFW_KEY_S;
                int moveLeft = GLFW_KEY_A;
                int moveRight = GLFW_KEY_D;
                int moveUp = GLFW_KEY_LEFT_SHIFT;
                int moveDown = GLFW_KEY_LEFT_CONTROL;
            };
            //delete copy constructor and copy assignment operator
            KeyboardListener(const KeyboardListener&) = delete;
            KeyboardListener& operator = (const KeyboardListener&) = delete;

            void moveInPlaneXZ(GLFWwindow *window, float deltaTime, OceanGameObject &gameObject);

            KeyMap keyMap{};
            float moveSpeed{2.5f};
            float turnSpeed{1.0f};
    };
}