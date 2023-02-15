#version 460

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main() {
    // param 1: index of the vertex in the array, param 2: z value, param 3: divider 
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}