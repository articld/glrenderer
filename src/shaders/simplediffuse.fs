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
    vec3 lightDirection = normalize(vec3(-2.0, 4.0, -1.0));
    vec3 ambient = vec3(0.05, 0.05, 0.05) * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * vec3(0.4, 0.4, 0.4) * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    vec3 color = ambient + diffuse;
    FragColor = vec4(color, 1.0);
}
