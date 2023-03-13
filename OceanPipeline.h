#pragma once

#include <string>
#include <vector>
#include "OceanDevice.h"

namespace ocean {
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo & operator = (const PipelineConfigInfo&) = delete;

        //VkViewport viewport;
        //VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo; //avoid pviewport become null pointer when copied
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo; //
        VkPipelineRasterizationStateCreateInfo rasterizerInfo; //
        VkPipelineMultisampleStateCreateInfo multisamplingInfo; //
        VkPipelineColorBlendAttachmentState colorBlendAttachment; //
        VkPipelineColorBlendStateCreateInfo colorBlendingInfo; //
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo; //
        std::vector<VkDynamicState> dynamicStates;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo; //
        VkPipelineDynamicStateCreateFlags dynamicStateFlags; //
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        uint32_t subpass = 0;
    };
    class OceanPipeline {
        public:
        //const is for value that cannot be changed in the runtime
            OceanPipeline(OceanDevice& device, const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo);
            ~OceanPipeline();
            //the poly construction function takes a oceanPipeline reference
            OceanPipeline(const OceanPipeline&) = delete;
            OceanPipeline& operator = (const OceanPipeline&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            //default configuration
            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        private:
            static std::vector<char> readFile(const std::string& filename);
            void createGraphicsPipeline(
                const std::string& vertShaderPath, 
                const std::string& fragShaderPath, 
                const PipelineConfigInfo& configInfo);
            //add shader module
            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            OceanDevice& device;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
}