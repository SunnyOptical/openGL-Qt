#version 410

in vec3 outColor;
in vec2 outUV;
in vec3 outNormal;
in vec3 fragPosition;
uniform sampler2D texture;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
float ambitStrength = 0.6;

vec2 circleCenter = vec2(0.0, 0.0);
float circleRadius = 0.3;

void main(){

    vec3 normal = normalize(outNormal);
    //计算环境光:环境光响应强度*环境光
    vec3 ambColor = lightColor * ambitStrength;
    //计算漫反射光:根据入射角度和法线的夹角，确定漫反射系数，再乘以光的颜色
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float diffStrength = max(dot(normal,lightDirection),0.0);
    vec3 diff = lightColor * diffStrength;
    //计算镜面反射:

    vec3 viewDirection = normalize(-fragPosition);
    vec3 refDirection = reflect(-lightDirection,normal);
    float specularStrength = pow(max(dot(viewDirection, refDirection), 0.0), 32);
    vec3 sp = lightColor * specularStrength;
    vec3 outColorFinally = (ambColor + diff + sp);

    float distanceToCenter = distance(vec2(fragPosition.xy), circleCenter);

    if(distanceToCenter > circleRadius){
        gl_FragColor =texture2D(texture,outUV)*vec4(0.3,0.3,0.3,1.0);
    }else{
        gl_FragColor =texture2D(texture,outUV)*vec4(outColorFinally.xyz,1.0);

    }

}
