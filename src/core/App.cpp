#include "App.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>

#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "render/OceanRenderSystem.h"
#include "input/KeyboardListener.h"
#include "camera/PerspectiveCamera.h"
#include "core/OceanBuffer.h"
#include "core/OceanFrameInfo.h"

namespace ocean {

    // struct for passing read-only data to pipeline shaders
    struct GlobalUBO{
        glm::mat4 projectionMatrix{1.f};
        glm::vec3 lightPosition = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
    }; 

    App::App()
    {
        // std::cout << "first cpp starts" << std::endl;
        loadGameObjects();
    }

    App::~App(){
        ;
    }

    std::string App::getName()
    {
        return NAME;
    }
    glm::vec2 App::getSize()
    {
        return glm::vec2{WIDTH, HEIGHT};
    }


    OceanGameObject App::singleGameObject(std::string filePath, glm::vec3 trainslation, glm::vec3 scale){
        std::shared_ptr<OceanModel> model = OceanModel::createModel(oceanDevice, filePath);
        auto gameObject = OceanGameObject::createGameObject();
        gameObject.model = model;
        gameObject.transform3d.translation = trainslation;
        gameObject.transform3d.scale = scale;//{.5f, .5f, .5f};

        return gameObject;
    }

    // TODO: multi-thread this process?
    void App::loadGameObjects(){
        std::vector<GameObjectArray> gameObjs = {
            // left/right, up/down, forward/backward
            {"models/smooth_vase.obj", glm::vec3(.0f, .5f, 2.0f), glm::vec3(3.f)}, 
            // {"models/flat_vase.obj", glm::vec3(3.5f, 1.99f, 3.5f), glm::vec3(2.f)}, 
            {"models/colored_cube.obj", glm::vec3(.0f, -1.f, 2.0f), glm::vec3(.1f)},
            // {"models/tar.obj", glm::vec3(.0f, 2.0f, .0f), glm::vec3(.1f)}
        };

        for (auto& gameObj : gameObjs) {
            std::shared_ptr<OceanModel> model = OceanModel::createModel(oceanDevice, gameObj.filePath);
            auto gameObject = OceanGameObject::createGameObject();
            gameObject.model = model;
            gameObject.transform3d.translation = gameObj.translation;
            gameObject.transform3d.scale = gameObj.scale;//{.5f, .5f, .5f};

            gameObjects.push_back(std::move(gameObject));
        }
    }

    void App::run() {
        OceanBuffer globalUboBuffer(
            oceanDevice,
            sizeof(GlobalUBO),
            OceanSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            oceanDevice.properties.limits.minUniformBufferOffsetAlignment
        );
        // enable writing to its memeory
        globalUboBuffer.map();

        // std::cout << "run starts" << std::endl;
        OceanRenderSystem renderSystem{ oceanDevice, oceanRenderer.getSwapChainRenderPass() } ;
        PerspectiveCamera camera{};
        // camera.setViewDirection(glm::vec3(-2.f, -2.f, -1.f), glm::vec3(1.f, 1.f, .5f)); //LESSON: this is in YXZ
        // std::cout << "render system created" << std::endl;
        // create time stamp
        auto time = std::chrono::high_resolution_clock::now();
        auto viewerObject = OceanGameObject::createGameObject();
        KeyboardListener keyboardListener{};
        while(!oceanWindow.shouldClose()) {
            //
            glfwPollEvents();
            
            auto newTime = std::chrono::high_resolution_clock::now();
            //float delta = std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count();
            float delta = std::chrono::duration<float, std::chrono::seconds::period>(newTime - time).count();
            time = newTime;

            //keyboard controller
            keyboardListener.moveInPlaneXZ(oceanWindow.getWindow(), delta, viewerObject); 
            camera.setViewYXZ(viewerObject.transform3d.translation, viewerObject.transform3d.rotation);

            float aspect = oceanRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 100.f);//always update with window size, left & right = aspect
            //the begin fram function will return a null function if the swap chain need to be recreated
            if(auto commandBuffer = oceanRenderer.beginFrame()) {
                int frameIndex = oceanRenderer.getFrameIndex();

                OceanFrameInfo frameInfo{
                    frameIndex,
                    delta,
                    commandBuffer,
                    camera
                };

                // update buffer data
                GlobalUBO ubo{};
                ubo.projectionMatrix = camera.getProjection() * camera.getView();
                globalUboBuffer.writeToBuffer(&ubo, frameIndex);
                globalUboBuffer.flushIndex(frameIndex);

                //begin offscreen shadow pass
                //render shadow castign objects
                //end offscreen shadow pass
                // std::cout << "0" <<std::endl;
                oceanRenderer.beginSwapChainRenderPass(commandBuffer);
                // std::cout << "1" << std::endl;
                renderSystem.renderGameObjects(frameInfo, gameObjects);
                // std::cout << "2" << std::endl;
                oceanRenderer.endSwapChainRenderPass(commandBuffer);
                // std::cout << "3" << std::endl;
                oceanRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(oceanDevice.device());
    }
}
