#include "first_app.h"
#include "OceanRenderSystem.h"
#include "KeyboardListener.h"
#include "PerspectiveCamera.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>
// #include <iostream>

namespace ocean {
    FirstApp::FirstApp()
    {
        // std::cout << "first cpp starts" << std::endl;
        loadGameObjects();
    }
    FirstApp::~FirstApp(){}

    std::unique_ptr<OceanModel> createCubeModel(OceanDevice& device, glm::vec3 offset) {
        OceanModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }
        
        modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        
        return std::make_unique<OceanModel>(device, modelBuilder);
    }

    void FirstApp::loadGameObjects(){
        std::shared_ptr<OceanModel> model = OceanModel::createModel(oceanDevice, "models/smooth_vase.obj");
        auto cube = OceanGameObject::createGameObject();
        cube.model = model;
        cube.transform3d.translation = {.0f, .0f, 2.5f};
        cube.transform3d.scale = glm::vec3(3.f);//{.5f, .5f, .5f};

        gameObjects.push_back(std::move(cube));
    }

    void FirstApp::run() {
        // std::cout << "run starts" << std::endl;
        OceanRenderSystem renderSystem{ oceanDevice, oceanRenderer.getSwapChainRenderPass() } ;
        PerspectiveCamera camera{};
        // camera.setViewDirection(glm::vec3(-2.f, -2.f, -1.f), glm::vec3(1.f, 1.f, .5f)); //LESSON: this is in YXZ
        // std::cout << "render system created" << std::endl;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto viewerObject = OceanGameObject::createGameObject();
        KeyboardListener keyboardListener{};
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            //float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            //keyboard controller
            keyboardListener.moveInPlaneXZ(oceanWindow.getWindow(), deltaTime, viewerObject); 
            camera.setViewYXZ(viewerObject.transform3d.translation, viewerObject.transform3d.rotation);

            float aspect = oceanRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 100.f);//always update with window size, left & right = aspect
            //the begin fram function will return a null function if the swap chain need to be recreated
            if(auto commandBuffer = oceanRenderer.beginFrame()) {
                //begin offscreen shadow pass
                //render shadow castign objects
                //end offscreen shadow pass
                // std::cout << "0" <<std::endl;
                oceanRenderer.beginSwapChainRenderPass(commandBuffer);
                // std::cout << "1" << std::endl;
                renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                // std::cout << "2" << std::endl;
                oceanRenderer.endSwapChainRenderPass(commandBuffer);
                // std::cout << "3" << std::endl;
                oceanRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(oceanDevice.device());
    }
}