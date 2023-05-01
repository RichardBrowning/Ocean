#version 460
//layout qualifier take positions, we only use 0 for now, actually support outputing to multiple locations
//out qualifier, define output variable type and name
layout (location = 0) in vec3 fragColor;
layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push{
    mat4 transform;
    vec3 color;
} push;

const float fogDensity = 0.01;// set the density of the fog
const float fogGradient = 2.0;

void main(){
    float distance = length(gl_FragCoord.xyz);
    
    // float fogAmount = 1.0 - exp(-pow((distance * fogDensity), fogGradient));
    // float fogFactor = exp(-0.02 * gl_FragCoord.z); // calculate the fog factor based on the depth of the fragment
    // outColor = vec4( fragColor* (20/distance), 1.0 );
    outColor = vec4( fragColor, 1.0 );
}