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
        //createPipeline();
        recreateSwapChain();
        createCommandBuffers();
    }
    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(oceanDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
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
        assert(oceanSwapChain && "Cannot create pipeline before swap chain!");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

        PipelineConfigInfo pipelineConfig{};
        OceanPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = oceanSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        oceanPipeline = std::make_unique<OceanPipeline>(oceanDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(oceanSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = oceanDevice.getCommandPool();
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        if(vkAllocateCommandBuffers(oceanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(oceanDevice.device(), oceanDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }

    void FirstApp::recordCommandBuffer(int imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        //if begin command buffer fails, throw an error
        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = oceanSwapChain->getRenderPass();
        renderPassInfo.framebuffer = oceanSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = oceanSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        //darker BG
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)oceanSwapChain->getSwapChainExtent().width;
        viewport.height = (float)oceanSwapChain->getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissors{{0, 0}, oceanSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissors);

        //oceanPipeline->bind(commandBuffers[imageIndex]);
        //oceanModel->bind(commandBuffers[imageIndex]);
        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void FirstApp::recreateSwapChain(){
        auto extent = oceanWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = oceanWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(oceanDevice.device());
        if (oceanSwapChain == nullptr)
            oceanSwapChain = std::make_unique<OceanSwapChain>(oceanDevice, extent);
        else{
            oceanSwapChain = std::make_unique<OceanSwapChain>(oceanDevice, extent, std::move(oceanSwapChain));
            if (oceanSwapChain -> imageCount() != commandBuffers.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        //if render pass compatible do nothing else
        createPipeline();
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

    void FirstApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = oceanSwapChain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        recordCommandBuffer(imageIndex);
        result = oceanSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || oceanWindow.wasWindowResized()) {
            oceanWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        //will be executed 
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}