#include "first_app.h"
#include <stdexcept>

namespace ocean {
    FirstApp::FirstApp()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }
    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while(!oceanWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr; //pipeline set layout pass data other than vertex data to the shaders
        pipelineLayoutInfo.pushConstantRangeCount = 0; 
        pipelineLayoutInfo.pPushConstantRanges = nullptr; //way to efficiently pass A SMALL AMOUNT OF DATA to shaders
        //device documentation ## 
        if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr/**allocation callback*/, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        auto pipelineConfig = OceanPipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
        pipelineConfig.renderPass = swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        oceanPipeline = std::make_unique<OceanPipeline>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void FirstApp::createCommandBuffers()
    {
    }
    void FirstApp::drawFrame()
    {
    }
}