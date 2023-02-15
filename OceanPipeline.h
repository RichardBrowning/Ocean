#pragma once
#include <string>
#include <vector>

namespace ocean {
    class OceanPipeline {
        public:
            OceanPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
        private:
            static std::vector<char> readFile(const std::string& filename);
            void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
    };
}