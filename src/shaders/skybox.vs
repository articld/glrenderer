#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vec4 rotatingTexCoords = vec4(aPos, 1.0) * model;
    TexCoord = rotatingTexCoords.xyz;
    //hey, funziona!
    mat4 viewNoTranslation = mat4(1.0);
    viewNoTranslation[0] = vec4(view[0][0], view[0][1], view[0][2], 0.0);
    viewNoTranslation[1] = vec4(view[1][0], view[1][1], view[1][2], 0.0);
    viewNoTranslation[2] = vec4(view[2][0], view[2][1], view[2][2], 0.0);
    viewNoTranslation[3] = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}