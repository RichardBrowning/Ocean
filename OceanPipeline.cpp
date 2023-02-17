#include "OceanPipeline.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace ocean {
    OceanPipeline::OceanPipeline(OceanDevice& device, const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo) : device{device} {
        createGraphicsPipeline(vertShaderPath, fragShaderPath, configInfo);
    }

    std::vector<char> OceanPipeline::readFile(const std::string& filename) {
        //ate = seek to the end of stream immediately after open, binary = open in binary mode
        std::ifstream file{filename, std::ios::ate | std::ios::binary};
        //if file cannot be opened, throw an exception
        if(!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        //get file size
        //static_cast is used to convert the type of the expression to the type specified by the cast
        //tellg() returns The current position of the get pointer on success
        size_t fileSize = static_cast<size_t>(file.tellg());
        //create a buffer to hold the file data
        std::vector<char> buffer(fileSize);
        //seek to the beginning of the file
        file.seekg(0);
        //read the file data into the buffer
        file.read(buffer.data(), fileSize);
        //close the file
        file.close();
        return buffer;
    }

    void OceanPipeline::createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo) {
        //variable’s data type will automatically be deducted from its initializer
        auto vertexShaderCode = readFile(vertShaderPath);
        auto fragmentShaderCode = readFile(fragShaderPath);

        std::cout << "Vertex Shader Size: " << vertexShaderCode.size() << std::endl;
        std::cout << "Fragment Shader Size: " << fragmentShaderCode.size() << std::endl;
    }
    void OceanPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        //reinterpret_cast is used to cast a pointer to any type of data
        //data() returns a pointer to the first element in the vector
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
    }
    PipelineConfigInfo OceanPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};

        return configInfo;
    }
}