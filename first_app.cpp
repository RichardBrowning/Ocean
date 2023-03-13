#include "first_app.h"
#define GLM_FORRCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <stdexcept>
#include <cassert>
#include <array>

namespace ocean {
    struct SimplePushConstantData {
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
        loadModel();
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
    void FirstApp::loadModel()
    {
        std::vector<OceanModel::Vertex> vertices = {
            {{0.0f, -0.5f},{1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        oceanModel = std::make_unique<OceanModel>(oceanDevice, vertices);
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
        /**
        for (int i = 0; i < commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            //if begin command buffer fails, throw an error
            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = oceanSwapChain->getRenderPass();
            renderPassInfo.framebuffer = oceanSwapChain->getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = oceanSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            oceanPipeline->bind(commandBuffers[i]);
            oceanModel->bind(commandBuffers[i]);
            oceanModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
         */
    }

    void FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(oceanDevice.device(), oceanDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }

    void FirstApp::recordCommandBuffer(int imageIndex){
        //animation
        static int frame = 0;
        frame = (frame+1)%1000;

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

        oceanPipeline->bind(commandBuffers[imageIndex]);
        oceanModel->bind(commandBuffers[imageIndex]);

        for (int j = 0; j < 4; j++){
            SimplePushConstantData push{};
            push.offset = {-0.5f + frame*0.002f, -0.4f + j*0.25f};
            push.color = {0.0f, 0.0f, 0.2f + j*0.2f};
            
            vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            oceanModel->draw(commandBuffers[imageIndex]);
        }

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