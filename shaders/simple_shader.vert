#version 460
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(push_constant) uniform Push{
    vec2 offset;
    vec3 color;
} push;

void main() {
    // param 1: index of the vertex in the array, param 2: z value, param 3: divider 
    gl_Position = vec4(position + push.offset, 0.0, 1.0);
}