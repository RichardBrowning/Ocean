#version 460

layout (location = 0) in vec2 position;


void main() {
    // param 1: index of the vertex in the array, param 2: z value, param 3: divider 
    gl_Position = vec4(position, 0.0, 1.0);
}