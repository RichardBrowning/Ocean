#include "OceanRenderSystem.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <cassert>
#include <array>

namespace ocean {
        // glm::vec3 offset; pushed to transaform with homogeneous coordinates
        //certain objects like uniform and push constants must be laid out to meet certain reqirements 
        // a scaler of size N has a scalar alignment of N
        // a 2 component vector has a base alignment equal to 2* its scalar alignment
        // scalar float N = 4 bytes, vec2 = 2*4 = 8 bytes, vec3 = 4N = 16 bytes
    struct SimplePushConstantData {
        glm::mat4 transform{1.f};
        // glm::vec3 offset; pushed to transaform with homogeneous coordinates
        alignas(16) glm::vec3 color{};
    };

    OceanRenderSystem::OceanRenderSystem(OceanDevice& device, VkRenderPass renderPass) : oceanDevice{device} {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    OceanRenderSystem::~OceanRenderSystem() {
        vkDestroyPipelineLayout(oceanDevice.device(), pipelineLayout, nullptr);
    }

    void OceanRenderSystem::createPipelineLayout() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(oceanDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void OceanRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        OceanPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        oceanPipeline = std::make_unique<OceanPipeline>(oceanDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",pipelineConfig);
    }

    void OceanRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<OceanGameObject>& gameObjects, const OceanPerspectiveCamera &camera) {
        oceanPipeline->bind(commandBuffer);
        auto projectionView = camera.getProjection() * camera.getView();
        for (auto& gameObject : gameObjects) {
            gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y + 0.01f, glm::two_pi<float>());
            gameObject.transform.rotation.x = glm::mod(gameObject.transform.rotation.x + 0.005f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.color = gameObject.color;
            //push.transform = camera.getProjection() * gameObject.transform.mat4();
            push.transform = projectionView * gameObject.transform.mat4();
            
            vkCmdPushConstants(commandBuffer,pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(SimplePushConstantData),&push);
            gameObject.model->bind(commandBuffer);
            gameObject.model->draw(commandBuffer);
        }
    }
}