#version 330 core

in VS_OUT{
    vec2 texCoords;
    vec3 normal;
    vec3 FragPosition;
    vec4 FragPosLightSpace;
}fs_in;

out vec4 FragColor;
uniform sampler2D shadowMap;

void main(){
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    FragColor = (1.0 - shadow) * vec4(0.3, 0.6, 0.1, 1.0);
}
