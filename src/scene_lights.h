#pragma once
#include "Shader.h"

void randomizeColor(glm::vec3 &randomLightColor) {
    randomLightColor.r = glm::linearRand(0.0f, 1.0f);
    randomLightColor.g = glm::linearRand(0.0f, 1.0f);
    randomLightColor.b = glm::linearRand(0.0f, 1.0f);
}

void scene_lights(Shader &shader, const glm::vec3 pointLightsPosition[]) {
    // directional light
    glm::vec3 dirLightPosition(glm::linearRand(-1.0f, 1.0f),
                               glm::linearRand(-1.0f, 1.0f),
                               glm::linearRand(-1.0f, 1.0f));

    shader.setVec3("dirLight.direction", dirLightPosition);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    glm::vec3 randomLightColor(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                               glm::linearRand(0.0f, 1.0f));
    shader.setVec3("dirLight.lightColor", randomLightColor);
    // point light 1
    shader.setVec3("pointLights[0].position", pointLightsPosition[0]);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);
    shader.setBool("pointLights[0].useThisLight", glm::linearRand(0.0f, 1.0f) > 0.5f);
    randomizeColor(randomLightColor);
    shader.setVec3("pointLights[0].lightColor", randomLightColor);
    // point light 2
    shader.setVec3("pointLights[1].position", pointLightsPosition[1]);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09f);
    shader.setFloat("pointLights[1].quadratic", 0.032f);
    shader.setBool("pointLights[1].useThisLight", glm::linearRand(0.0f, 1.0f) > 0.5f);
    randomizeColor(randomLightColor);
    shader.setVec3("pointLights[1].lightColor", randomLightColor);
    // point light 3
    shader.setVec3("pointLights[2].position", pointLightsPosition[2]);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09f);
    shader.setFloat("pointLights[2].quadratic", 0.032f);
    shader.setBool("pointLights[2].useThisLight", glm::linearRand(0.0f, 1.0f) > 0.5f);
    randomizeColor(randomLightColor);
    shader.setVec3("pointLights[2].lightColor", randomLightColor);
    // point light 4
    shader.setVec3("pointLights[3].position", pointLightsPosition[3]);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09f);
    shader.setFloat("pointLights[3].quadratic", 0.032f);
    shader.setBool("pointLights[3].useThisLight", glm::linearRand(0.0f, 1.0f) > 0.5f);
    randomizeColor(randomLightColor);
    shader.setVec3("pointLights[3].lightColor", randomLightColor);
}

