#include "KeyboardListener.h"
#include <limits>

namespace ocean{
    void KeyboardListener::moveInPlaneXZ(GLFWwindow *window, float deltaTime, OceanGameObject &gameObject){
        glm::vec3 rotate{0};
        if(glfwGetKey(window, keyMap.lookUp) == GLFW_PRESS){
            rotate.x += 1.f;
        }
        if(glfwGetKey(window, keyMap.lookDown) == GLFW_PRESS){
            rotate.x -= 1.f;
        }
        if(glfwGetKey(window, keyMap.lookLeft) == GLFW_PRESS){
            rotate.y -= 1.f;
        }
        if(glfwGetKey(window, keyMap.lookRight) == GLFW_PRESS){
            rotate.y += 1.f;
        }

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()){ //epsilon represents the smallest number that can be represented
            //rotate = glm::normalize(rotate);
            gameObject.transform3d.rotation += glm::normalize(rotate) * turnSpeed * deltaTime; //doesn't rotate diagnally than only one axis is moved
        }
        gameObject.transform3d.rotation.x = glm::clamp(gameObject.transform3d.rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
        gameObject.transform3d.rotation.y = glm::mod(gameObject.transform3d.rotation.y, glm::two_pi<float>());

        const glm::vec3 currentDir {sin(gameObject.transform3d.rotation.y), 0, cos(gameObject.transform3d.rotation.y)};
        const glm::vec3 rightDir {currentDir.z, 0, -currentDir.x};
        const glm::vec3 upDir {0, -1, 0};

        glm::vec3 moveDir{0.f};

        if(glfwGetKey(window, keyMap.moveForward) == GLFW_PRESS){
            moveDir += currentDir;
        }
        if(glfwGetKey(window, keyMap.moveBackward) == GLFW_PRESS){
            moveDir -= currentDir;
        }
        if(glfwGetKey(window, keyMap.moveLeft) == GLFW_PRESS){
            moveDir -= rightDir;
        }
        if(glfwGetKey(window, keyMap.moveRight) == GLFW_PRESS){
            moveDir += rightDir;
        }
        if(glfwGetKey(window, keyMap.moveUp) == GLFW_PRESS){
            moveDir += upDir;
        }
        if(glfwGetKey(window, keyMap.moveDown) == GLFW_PRESS){
            moveDir -= upDir;
        }

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()){
            moveDir = glm::normalize(moveDir);
            gameObject.transform3d.translation += glm::normalize(moveDir) * moveSpeed * deltaTime;
        }
    }
}