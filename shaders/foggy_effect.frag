#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(set = 1, binding = 0) uniform FogParameters {
    float fogStart;
    float fogEnd;
    vec3 fogColor;
} fogParams;

void main() {
    // Calculate the distance from the fragment to the camera
    float distance = length(fragPos - vec3(ubo.view[3]));

    // Calculate the fog factor based on the distance
    float fogFactor = (distance - fogParams.fogStart) / (fogParams.fogEnd - fogParams.fogStart);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Apply the fog factor to the fragment color
    vec3 foggedColor = mix(fragColor, fogParams.fogColor, fogFactor);

    outColor = vec4(foggedColor, 1.0);
}
