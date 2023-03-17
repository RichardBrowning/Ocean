#include "OceanModel.h"
#include <cassert>
#include <cstring>

namespace ocean {
    OceanModel::OceanModel(OceanDevice &device, const std::vector<Vertex> &vertices) : oceanDevice{device} //initialize "device" member variable
    {
        createVertexBuffer(vertices);
    }

    OceanModel::~OceanModel()
    {
        vkDestroyBuffer(oceanDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), vertexBufferMemory, nullptr);
    }

    void OceanModel::createVertexBuffer(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory);

        void *data;
        vkMapMemory(oceanDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(oceanDevice.device(), vertexBufferMemory);
    }


    void OceanModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }
    void OceanModel::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    std::vector<VkVertexInputBindingDescription> OceanModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> OceanModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> inputAttributeDescription(2);
        inputAttributeDescription[0].binding = 0;
        inputAttributeDescription[0].location = 0;
        inputAttributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
        inputAttributeDescription[0].offset = offsetof(Vertex, position);

        inputAttributeDescription[1].binding = 0;
        inputAttributeDescription[1].location = 1;
        inputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        inputAttributeDescription[1].offset = offsetof(Vertex, color);//calculates the offset from the first element of the structure to the color element
        return inputAttributeDescription;
    }
}