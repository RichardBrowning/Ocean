#include "first_app.h"
#include "OceanRenderSystem.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <cassert>
#include <array>
#include <iostream>

namespace ocean {
    FirstApp::FirstApp()
    {
        std::cout << "first cpp starts" << std::endl;
        loadGameObjects();
    }
    FirstApp::~FirstApp(){}

    void FirstApp::loadGameObjects(){
        std::vector<OceanModel::Vertex> vertices = {
            {{0.0f, -0.5f},{1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        auto oceanModel = std::make_shared<OceanModel>(FirstApp::oceanDevice, vertices);

        auto triangle = OceanGameObject::createGameObject();
        triangle.model = oceanModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
        std::cout << "game objects loaded" << std::endl;
    }

    void FirstApp::run() {
        std::cout << "run starts" << std::endl;
        OceanRenderSystem renderSystem{ oceanDevice, oceanRenderer.getSwapChainRenderPass() } ;
        std::cout << "render system created" << std::endl;
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
            //the begin fram function will return a null function if the swap chain need to be recreated
            if(auto commandBuffer = oceanRenderer.beginFrame()) {
                //begin offscreen shadow pass
                //render shadow castign objects
                //end offscreen shadow pass
                // std::cout << "0" <<std::endl;
                oceanRenderer.beginSwapChainRenderPass(commandBuffer);
                // std::cout << "1" << std::endl;
                renderSystem.renderGameObjects(commandBuffer, gameObjects);
                // std::cout << "2" << std::endl;
                oceanRenderer.endSwapChainRenderPass(commandBuffer);
                // std::cout << "3" << std::endl;
                oceanRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(oceanDevice.device());
    }
}