#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform sampler2D skybox;

void main(){
    vec3 direction = normalize(TexCoord);
    float phi = atan(direction.z, direction.x);
    float theta = acos(direction.y);
    vec2 coord = vec2((phi / (2.0 * 3.14159265359)), theta / 3.14159265359);
    FragColor = texture(skybox, coord);
}