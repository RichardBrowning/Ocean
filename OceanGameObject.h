#pragma once
#include "OceanModel.h"
#include <memory>

namespace ocean{
    struct TransformComponent {
        glm::vec3 translation {};
        glm::vec3 scale {1.f, 1.f, 1.f};
        glm::vec3 rotation {};
        glm::mat4 mat4() {
            auto transform = glm::translate(glm::mat4{1.f}, translation);
            transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
            transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
            transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
            transform = glm::scale(transform, scale);

            return transform;
        }
    };

    class OceanGameObject{
    public:
        using id_t = unsigned int;

        static OceanGameObject createGameObject(){
            static id_t currentId = 0;
            return OceanGameObject{currentId++};
        }
        OceanGameObject(const OceanGameObject& other) = delete;
        OceanGameObject& operator = (const OceanGameObject& other) = delete;
        OceanGameObject(OceanGameObject && ) = default;
        OceanGameObject& operator = (OceanGameObject &&) = default;

        id_t getId() const { return id; }
        
        std::shared_ptr<OceanModel> model{};
        glm::vec3 color{};
        TransformComponent transform3d{};

    private:
        OceanGameObject(id_t id) : id{id} {}
        id_t id;

    };
}