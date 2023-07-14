#version 410
out vec4 outColor;
in vec2 outUV;
uniform sampler2D texture;
void main(){


    outColor = vec4(0.2,0.6,0.8,1.0);
    //outColor = texture2D(texture,outUV);

}
