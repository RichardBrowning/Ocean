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

        createShaderModule(vertexShaderCode, &vertShaderModule);
        createShaderModule(fragmentShaderCode, &fragShaderModule);
        //std::cout << "Vertex Shader Size: " << vertexShaderCode.size() << std::endl;
        //std::cout << "Fragment Shader Size: " << fragmentShaderCode.size() << std::endl;

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = VK_NULL_HANDLE;
        shaderStages[0].pSpecializationInfo = VK_NULL_HANDLE;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = VK_NULL_HANDLE;
        shaderStages[1].pSpecializationInfo = VK_NULL_HANDLE;
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

        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = static_cast<float>(width);
        //TODO: squashed triangle
        configInfo.viewport.height = static_cast<float>(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;

        configInfo.scissor.offset = {0, 0};
        configInfo.scissor.extent = {width, height};

        // combine the viewport and scissor into a viewport state
        configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportInfo.viewportCount = 1;
        configInfo.viewportInfo.pViewports = &configInfo.viewport;
        configInfo.viewportInfo.scissorCount = 1;
        configInfo.viewportInfo.pScissors = &configInfo.scissor;

        /**rasterization info*/ 
        configInfo.rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizerInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizerInfo.lineWidth = 1.0f;
        configInfo.rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        configInfo.rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizerInfo.depthBiasEnable = VK_FALSE;
        // Optional
        configInfo.rasterizerInfo.depthBiasConstantFactor = 0.0f;
        configInfo.rasterizerInfo.depthBiasClamp = 0.0f;
        configInfo.rasterizerInfo.depthBiasSlopeFactor = 0.0f;

        /**multisampling info*/
        configInfo.multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisamplingInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        // Optional
        configInfo.multisamplingInfo.minSampleShading = 1.0f;
        configInfo.multisamplingInfo.pSampleMask = nullptr;
        configInfo.multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
        configInfo.multisamplingInfo.alphaToOneEnable = VK_FALSE;

        /**color blending info*/
        configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        configInfo.colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendingInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
        configInfo.colorBlendingInfo.attachmentCount = 1;
        configInfo.colorBlendingInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendingInfo.blendConstants[0] = 0.0f;
        configInfo.colorBlendingInfo.blendConstants[1] = 0.0f;
        configInfo.colorBlendingInfo.blendConstants[2] = 0.0f;
        configInfo.colorBlendingInfo.blendConstants[3] = 0.0f;

        /**depth testing*/
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        // Optional
        configInfo.depthStencilInfo.front = {};
        configInfo.depthStencilInfo.back = {};

        return configInfo;
    }
}