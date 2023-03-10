#version 460

//layout qualifier take positions, we only use 0 for now, actually support outputing to multiple locations
//out qualifier, define output variable type and name
layout (location = 0) out vec4 outColor;

void main(){
    outColor = vec4(1.0, 1.0, 0.0, 1.0);
}