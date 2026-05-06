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
}vs_out;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;


uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //per l'illuminazione si usa le coordinate del mondo
    FragPosition = vec3(model * vec4(aPos, 1.0));
    vs_out.texCoords = aTexCoord;
    //questa operazione è meglio fatta in cpu che in gpu
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
