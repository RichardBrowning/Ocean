#pragma once
#include "OceanModel.h"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace ocean{
    // struct TransformComponent {
    //     glm::vec3 translation{};
    //     glm::vec3 scale {1.f, 1.f, 1.f};
    //     glm::vec3 rotation{};
    //     //matrix correcponds to translate * rx * ry * rz * scale transformation
    //     //rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
    //     glm::mat4 mat4(){
    //         auto transform = glm::translate(glm::mat4{1.f}, translation);
    //         transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
    //         transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
    //         transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
    //         transform = glm::scale(transform,scale);
    //         return transform;
    //     }
    // };

    struct Transform2dComponent {
    glm::vec2 translation{};  // (position offset)
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2() {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMatrix{{c, s}, {-s, c}};

        glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
        return rotMatrix * scaleMat;
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
        Transform2dComponent transform2d{};

    private:
        OceanGameObject(id_t id) : id{id} {}
        id_t id;

    };
}