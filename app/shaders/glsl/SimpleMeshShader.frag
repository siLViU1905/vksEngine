#version 460

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoords;
layout(location = 3) in vec3 viewPos;
layout(location = 4) in mat3 TBN;

layout(location = 0) out vec4 outColor;

struct DirectionalLight
{
    vec3 diffuse;

    vec3 specular;

    vec3 ambient;

    float intensity;

    vec3 direction;
};

layout(set = 0, binding = 1) uniform DirectionalLightsUBO
{
    DirectionalLight directionalLights[10];
};

struct PointLight
{
    vec3 diffuse;

    vec3 specular;

    vec3 ambient;

    float intensity;

    vec3 position;

    float constant;

    float linear;

    float quadratic;
};

layout(set = 0, binding = 2) uniform PointLightsUBO
{
    PointLight pointLights[10];
};

layout(binding = 3) uniform Counters
{
    int pointLightCounter;

    int directionalLightCounter;
}
counters;

layout(push_constant) uniform PushConstants {
    vec4 color;
} pc;

struct LightComponent
{
    vec3 diffuse;

    vec3 specular;

    vec3 ambient;
};

void calculateDirectionalLight(int index, vec3 normal, vec3 viewDir, out vec3 ambient, out vec3 diffuse, out vec3 specular);

void calculatePointLight(int index, vec3 normal, vec3 viewDir, out vec3 ambient, out vec3 diffuse, out vec3 specular);

void calculateLights(vec3 normal, vec3 viewDir, out LightComponent totalPointLight, out LightComponent totalDirectionalLigh);

void main()
{
    vec3 diffuseColor = pc.color.rgb;

    vec3 viewDir = normalize(viewPos - fragPos);

    LightComponent totalDirectionalLight;

    LightComponent totalPointLight;

    calculateLights(fragNormal, viewDir, totalPointLight, totalDirectionalLight);

    vec3 totalAmbient = totalPointLight.ambient + totalDirectionalLight.ambient;
    vec3 totalDiffuse = totalPointLight.diffuse + totalDirectionalLight.diffuse;
    vec3 totalSpecular = totalPointLight.specular + totalDirectionalLight.specular;

    vec3 finalColor = (totalAmbient + totalDiffuse) * diffuseColor + totalSpecular;

    outColor = vec4(finalColor, 1.0);
}

void calculateDirectionalLight(int index, vec3 normal, vec3 viewDir, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
    ambient = directionalLights[index].ambient * directionalLights[index].intensity;

    vec3 lightDir = directionalLights[index].direction;

    float diff = max(dot(normal, lightDir), 0.0);

    diffuse = directionalLights[index].diffuse * diff * directionalLights[index].intensity;

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    specular = directionalLights[index].specular * spec * directionalLights[index].intensity;
}

void calculatePointLight(int index, vec3 normal, vec3 viewDir, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{

    vec3 lightDir = normalize(pointLights[index].position - fragPos);

    ambient = pointLights[index].ambient * pointLights[index].intensity;

    float diff = max(dot(normal, lightDir), 0.0);

    diffuse = pointLights[index].diffuse * diff * pointLights[index].intensity;

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    specular = pointLights[index].specular * spec * pointLights[index].intensity;

    float distance = length(pointLights[index].position - fragPos);

    float attenuation = 1.0 / (pointLights[index].constant + pointLights[index].linear * distance + pointLights[index].quadratic * (distance * distance));

    diffuse *= attenuation;

    specular *= attenuation;

    ambient *= attenuation;
}

void calculateLights(vec3 normal, vec3 viewDir, out LightComponent totalPointLight, out LightComponent totalDirectionalLight)
{
    totalPointLight.diffuse = totalPointLight.ambient = totalPointLight.specular = vec3(0.0);

    totalDirectionalLight.diffuse = totalDirectionalLight.ambient = totalDirectionalLight.specular = vec3(0.0);

    if(counters.pointLightCounter > 0)
        for(int i = 0; i < counters.pointLightCounter; ++i)
        {
            vec3 diffuse, specular, ambient;

            calculatePointLight(i, normal, viewDir, ambient, diffuse, specular);

            totalPointLight.diffuse += diffuse;

            totalPointLight.specular += specular;

            totalPointLight.ambient += ambient;
        }

    if(counters.directionalLightCounter > 0)
        for(int i = 0; i < counters.directionalLightCounter; ++i)
        {
            vec3 diffuse, specular, ambient;

            calculateDirectionalLight(i, normal, viewDir, ambient, diffuse, specular);

            totalDirectionalLight.diffuse += diffuse;

            totalDirectionalLight.specular += specular;

            totalDirectionalLight.ambient += ambient;
        }
}