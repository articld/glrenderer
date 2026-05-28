#version 330 core

in VS_OUT{
    vec2 texCoords;
    vec3 normal;
    vec3 FragPosition;
    vec4 FragPosLightSpace;
}fs_in;

out vec4 FragColor;
uniform sampler2D shadowMap;
uniform vec3 lightDir;

void main(){
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(fs_in.normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x<=1; ++x){
        for(int y = -1; y<=1; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;


    FragColor = (1.0 - shadow) * vec4(0.3, 0.6, 0.1, 1.0);
}
