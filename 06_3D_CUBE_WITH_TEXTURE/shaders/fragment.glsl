#version 410
out vec4 outColor;
in vec2 outUV;
uniform sampler2D texture;
void main(){
    outColor =texture2D(texture,outUV);
}
