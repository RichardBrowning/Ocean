#pragma once

#include "OceanWindow.h"
#include "OceanPipeline.h"
#include "OceanDevice.h"

namespace ocean {
    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            const std::string NAME = "First App";

            void run();
        private:
           OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
           OceanDevice device{oceanWindow};
           OceanPipeline oceanPipeline{device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", OceanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}