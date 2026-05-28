#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 texCoords;
    vec3 normal;
    vec3 FragPosition;
    vec4 FragPosLightSpace;
}fs_in;

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

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace){
    //divisione prospettica. Non serve per il caso in cui si usi una proiezione ortogonale
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //passo da coordinate NDC [-1, 1] a coordiante in [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(fs_in.normal, dirLight.direction)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x<=1; ++x){
        for(int y = -1; y<=1; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    //uso il modello di shading di blinn phong
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, fs_in.texCoords)).r * spec * light.specular;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

    return lighting;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance +
                              light.quadratic * (distance * distance));
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    //diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    //specular, calcolo la riflessione della luce dot posizione camera
    //reflect si aspetta che il primo vettore punti dalla sorgente di luce alla posizione del fragment
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, fs_in.texCoords)).r * spec * light.specular;

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
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.texture_specular1, fs_in.texCoords)).r * spec * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDirection = normalize(viewPos - fs_in.FragPosition);

    vec3 result = CalcDirLight(dirLight, norm , viewDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPosition, viewDirection);

    result += CalcSpotLight(spotLight, norm, fs_in.FragPosition, viewDirection);

    FragColor = vec4(result, 1.0);
}
