#include "OceanModel.h"
#include <cassert>
#include <cstring>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace ocean {
    OceanModel::OceanModel(OceanDevice &device, const OceanModel::Builder &builder) : oceanDevice{device} //initialize "device" member variable
    {
        createVertexBuffer(builder.vertices);
        createIndexBuffer(builder.indices);
    }

    OceanModel::~OceanModel()
    {
        vkDestroyBuffer(oceanDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), vertexBufferMemory, nullptr);
        if (hasIndexBuffer){
            vkDestroyBuffer(oceanDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(oceanDevice.device(), indexBufferMemory, nullptr);
        }
    }
    void OceanModel::createIndexBuffer(const std::vector<uint32_t> &indices){
        indexCount = static_cast<uint32_t>(indices.size());
        if (indexCount != 0)
            hasIndexBuffer = true;
        if (!hasIndexBuffer)
            return;
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(oceanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(oceanDevice.device(), stagingBufferMemory);

        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory);

        //copy staging buffer to index buffer
        oceanDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        //cleanup staging buffer
        vkDestroyBuffer(oceanDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), stagingBufferMemory, nullptr);
    }

    void OceanModel::createVertexBuffer(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(oceanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(oceanDevice.device(), stagingBufferMemory);

        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory);

        //copy staging buffer to vertex buffer
        oceanDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        //cleanup staging buffer
        vkDestroyBuffer(oceanDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), stagingBufferMemory, nullptr);
    }

    void OceanModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (hasIndexBuffer)
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
    void OceanModel::draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer)
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        else
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
        inputAttributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        inputAttributeDescription[0].offset = offsetof(Vertex, position);

        inputAttributeDescription[1].binding = 0;
        inputAttributeDescription[1].location = 1;
        inputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        inputAttributeDescription[1].offset = offsetof(Vertex, color);//calculates the offset from the first element of the structure to the color element
        return inputAttributeDescription;
    }
}