#include "first_app.h"
#include "OceanRenderSystem.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <cassert>
#include <array>
// #include <iostream>

namespace ocean {
    FirstApp::FirstApp()
    {
        // std::cout << "first cpp starts" << std::endl;
        loadGameObjects();
    }
    FirstApp::~FirstApp(){}

    std::unique_ptr<OceanModel> createCubeModel(OceanDevice& device, glm::vec3 offset) {
        std::vector<OceanModel::Vertex> vertices{
        
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        
        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<OceanModel>(device, vertices);
    }

    void FirstApp::loadGameObjects(){
        std::shared_ptr<OceanModel> model = createCubeModel(FirstApp::oceanDevice, glm::vec3{.0f, .0f, .0f});
        auto cube = OceanGameObject::createGameObject();
        cube.model = model;
        cube.transform3d.translation = {.0f, .0f, .0f};
        cube.transform3d.scale = {.5f, .5f, .5f};

        gameObjects.push_back(std::move(cube));
    }

    void FirstApp::run() {
        // std::cout << "run starts" << std::endl;
        OceanRenderSystem renderSystem{ oceanDevice, oceanRenderer.getSwapChainRenderPass() } ;
        PerspectiveCamera camera{};
        camera.setViewDirection(glm::vec3(-2.f, -2.f, -1.f), glm::vec3(1.f, 1.f, .5f));
        // std::cout << "render system created" << std::endl;
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
            float aspect = oceanRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 100.f);
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