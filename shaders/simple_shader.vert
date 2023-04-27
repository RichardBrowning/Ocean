#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (location = 0) out vec3 fragColor;
layout (push_constant) uniform Push{
    mat4 transform;
    mat4 normalMatrix;
    //vec3 color;
} push;

const vec3 LIGHT_DIRECTION = normalize(vec3(-1.0, 3.0, 1.0));
const float AMBIENT_INTENSITY = 0.1;

void main() {
    // param 1: index of the vertex in the array, param 2: z value, param 3: divider 
    gl_Position = push.transform * vec4(position, 1.0);
    // fragColor = color;
    vec3 normalWorldSpace = normalize(vec3(push.normalMatrix * vec4(normal, 0.0)));
    float lightIntensity = AMBIENT_INTENSITY + max(dot(normalWorldSpace, -LIGHT_DIRECTION), 0.0);

    fragColor = lightIntensity * color;
}