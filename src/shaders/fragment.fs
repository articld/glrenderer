#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

struct Material {
    //deve essere definito in un uniform, altrimenti da errore
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3 viewPos;

void main()
{
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

    //specular, calcolo la riflessione della luce dot posizione camera
    vec3 viewDirection = normalize(viewPos - FragPosition);
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 reflectDirection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}