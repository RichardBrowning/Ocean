#include "OceanRenderer.h"
#include <stdexcept>
#include <cassert>
#include <array>
// #include <iostream>

namespace ocean{
    OceanRenderer::OceanRenderer(OceanWindow &window, OceanDevice &device) : oceanWindow(window), oceanDevice(device){
        recreateSwapChain();
        createCommandBuffers();
    }

    OceanRenderer::~OceanRenderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer OceanRenderer::beginFrame(){
        // std::cout << "start begin frame"<<std::endl;
        assert(!isFrameStarted && "Cannot call beginFrame while already in a progress.");
        // std::cout << "in the mid" << std::endl;
        auto result = oceanSwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("failed to acquire swap chain image!");
        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // if begin command buffer fails, throw an error
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");
        // std::cout << "end begin frame"<<std::endl;
        return commandBuffer;
    }

    void OceanRenderer::endFrame()
    {
        assert(isFrameStarted && "Cannot call endFrame while not in a progress.");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");

        auto result = oceanSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || oceanWindow.wasWindowResized()) {
            oceanWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        //will be executed 
        else if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present swap chain image!");
        OceanRenderer::isFrameStarted = false;
        OceanRenderer::currentFrameIndex = (OceanRenderer::currentFrameIndex + 1) % OceanSwapChain::MAX_FRAMES_IN_FLIGHT; 
    }

    void OceanRenderer::createCommandBuffers()
    {
        // commandBuffers.resize(oceanSwapChain->imageCount());
        commandBuffers.resize(OceanSwapChain::MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = oceanDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        if(vkAllocateCommandBuffers(oceanDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void OceanRenderer::recreateSwapChain(){
        auto extent = OceanRenderer::oceanWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = OceanRenderer::oceanWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(OceanRenderer::oceanDevice.device());
        if (OceanRenderer::oceanSwapChain == nullptr)
            OceanRenderer::oceanSwapChain = std::make_unique<OceanSwapChain>(OceanRenderer::oceanDevice, extent);
        else{
            std::shared_ptr<OceanSwapChain> oldSwapChain = std::move(OceanRenderer::oceanSwapChain);
            OceanRenderer::oceanSwapChain = std::make_unique<OceanSwapChain>(OceanRenderer::oceanDevice, extent, oldSwapChain);
            if (!oldSwapChain->compareSwapchainFormats(*OceanRenderer::oceanSwapChain.get())){
                throw std::runtime_error("Swapchain image or depth format has changed.");
            }
            // if (OceanRenderer::oceanSwapChain -> imageCount() != OceanRenderer::commandBuffers.size()) {
            //     freeCommandBuffers();
            //     createCommandBuffers();
            // }
        }
    }

    void OceanRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(oceanDevice.device(), oceanDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void OceanRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cannot call beginSwapChainRenderPass while not in a progress.");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from a different frame.");
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = oceanSwapChain->getRenderPass();
        renderPassInfo.framebuffer = oceanSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = oceanSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        //darker BG
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(oceanSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(oceanSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, oceanSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void OceanRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cannot call endSwapChainRenderPass while not in a progress.");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end render pass on command buffer from a different frame.");

        vkCmdEndRenderPass(commandBuffer);
    }
}