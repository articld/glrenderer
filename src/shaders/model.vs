#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
    vec2 texCoords;
    vec3 normal;
    vec3 FragPosition;
}vs_out;

out vec3 Normal;


uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.FragPosition = vec3(model * vec4(aPos, 1.0));
    vs_out.texCoords = aTexCoord;
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
}
