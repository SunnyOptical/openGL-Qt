#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

out vec2 outUV;
out vec3 outNormal; //输出给片段着色器的最终法线
out vec3 lightDirection;
out vec3 fragPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;
uniform vec3 cameraDirection;





//冯氏光照模型中包含环境光，漫反射，和镜面反射三个部分
void main(){

    gl_Position = projection * view * model * vec4(inPosition.xyz,1.0);
    //因为有叠加模型转换矩阵，所以需要求出变化后的法线
    vec3 afterTransformNormal = mat3(transpose(inverse(model)))*inNormal;
    fragPosition = vec3(model * vec4(inPosition.xyz,1.0));
    outNormal = afterTransformNormal;

    outUV = inUV;
}
