//Tengo lo stesso impianto di model.fs per ragioni di comodità, alla fine non mi dovrebbe servire cose particolari
#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 texCoords;
    vec3 normal;
    vec3 FragPosition;
    vec4 FragPosLightSpace;
}fs_in;

//deve essere definito in un uniform, altrimenti da errore
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular3;
    sampler2D texture_diffuse4;
    sampler2D texture_specular4;

    float shininess;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse1, fs_in.texCoords);
}
