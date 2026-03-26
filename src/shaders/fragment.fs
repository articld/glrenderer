#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
in vec3 LightPosition;

uniform sampler2D texture1;
uniform sampler2D texture2;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    //ambient
    vec3 ambient = material.ambient * light.ambient;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPosition - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    //specular, calcolo la riflessione della luce dot posizione camera
    vec3 viewDirection = normalize(- FragPosition);
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 reflectDirection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}