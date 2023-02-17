#pragma once

#include <string>
#include <vector>
#include "OceanDevice.h"

namespace ocean {
    struct PipelineConfigInfo {

    };
    class OceanPipeline {
        public:
        //const is for value that cannot be changed in the runtime
            OceanPipeline(OceanDevice& device, const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo);
            ~OceanPipeline() {}
            //the poly construction function takes a oceanPipeline reference
            OceanPipeline(const OceanPipeline&) = delete;
            void operator = (const OceanPipeline&) = delete;
            //default configuration
            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

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