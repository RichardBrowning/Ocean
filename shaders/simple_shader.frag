#version 460
//layout qualifier take positions, we only use 0 for now, actually support outputing to multiple locations
//out qualifier, define output variable type and name
layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push{
    mat2 transform;
    vec2 offset;
    vec3 color;
} push;

void main(){
    outColor = vec4( push.color, 1.0 );
}