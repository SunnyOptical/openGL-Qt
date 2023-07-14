#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

out vec2 outUV;
void main(){

    gl_Position = vec4(inPosition.xyz,1.0);
    outUV = inUV;
}
