#include "first_app.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <cassert>
#include <array>

namespace ocean {
    struct SimplePushConstantData {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        //certain objects like uniform and push constants must be laid out to meet certain reqirements 
        // a scaler of size N has a scalar alignment of N
        // a 2 component vector has a base alignment equal to 2* its scalar alignment
        // scalar float N = 4 bytes, vec2 = 2*4 = 8 bytes, vec3 = 4N = 16 bytes
        alignas(16) glm::vec3 color;
        float scale;
    };
    FirstApp::FirstApp()
    {
        loadGameObjects();
        createPipelineLayout();
        createPipeline();
    }
    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(oceanDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();

            if(auto commandBuffer = oceanRenderer.beginFrame()) {
                //begin offscreen shadow pass
                //render shadow castign objects
                //end offscreen shadow pass
                oceanRenderer.beginSwapChainRenderPass(commandBuffer);
                renderGameObjects(commandBuffer);
                oceanRenderer.endSwapChainRenderPass(commandBuffer);
                oceanRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(oceanDevice.device());
    }
    void FirstApp::loadGameObjects(){
        std::vector<OceanModel::Vertex> vertices = {
            {{0.0f, -0.5f},{1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        auto oceanModel = std::make_shared<OceanModel>(oceanDevice, vertices);

        auto triangle = OceanGameObject::createGameObject();
        triangle.model = oceanModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
    void FirstApp::createPipelineLayout()
    {
        
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr; //pipeline set layout pass data other than vertex data to the shaders
        pipelineLayoutInfo.pushConstantRangeCount = 1; 
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; //way to efficiently pass A SMALL AMOUNT OF DATA to shaders
        //device documentation ## 
        if(vkCreatePipelineLayout(oceanDevice.device(), &pipelineLayoutInfo, nullptr/**allocation callback*/, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

        PipelineConfigInfo pipelineConfig{};
        OceanPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = oceanRenderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        oceanPipeline = std::make_unique<OceanPipeline>(oceanDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
    {
        oceanPipeline->bind(commandBuffer);
        for (auto& gameObject : gameObjects)
        {
            gameObject.transform2d.rotation = glm::mod(gameObject.transform2d.rotation + 0.01f, glm::two_pi<float>());
            SimplePushConstantData push{};
            push.offset = gameObject.transform2d.translation;
            push.color = gameObject.color;
            push.transform = gameObject.transform2d.mat2();
            
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            gameObject.model->bind(commandBuffer);
            gameObject.model->draw(commandBuffer);
        }
    }

}