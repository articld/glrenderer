#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform vec3 viewPos;

//deve essere definito in un uniform, altrimenti da errore
struct Material {
    //probabilmente esiste un modo migliore, magari con array. Per il momento lascio così.
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

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float fallOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPosition);

    vec3 result = CalcDirLight(dirLight, norm , viewDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPosition, viewDirection);

    result += CalcSpotLight(spotLight, norm, FragPosition, viewDirection);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord));

    //specular, calcolo la riflessione della luce dot posizione camera
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 reflectDirection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, TexCoord)) * spec * light.specular;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance +
                              light.quadratic * (distance * distance));
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

    //diffuse
    vec3 lightDir = normalize(light.position - FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord));

    //specular, calcolo la riflessione della luce dot posizione camera
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 reflectDirection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, TexCoord)) * spec * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance +
                              light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.fallOff;
    float intensity = clamp((theta - light.fallOff) / epsilon, 0.0, 1.0);

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord));

    //specular, calcolo la riflessione della luce dot posizione camera
    vec3 viewDirection = normalize(viewPos - fragPos);
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 reflectDirection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, TexCoord)) * spec * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}