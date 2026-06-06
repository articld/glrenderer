#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/random.hpp>

#include "Shader.h"
#include "Camera.h"

#include "modelimport/model.h"

/*
#include "primitives/cube.h"
#include "primitives/plane.h"
#include "primitives/verticalplane.h"
#include "primitives/skybox.h"
*/

#include <iostream>
#include <vector>
#include <filesystem>

/*
unsigned int loadCubemap(std::vector<std::string> texture_faces);
*/

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;

constexpr float FOV = 39.0f;

// camera
//TODO: probabilmente mettere la posizione della camera a metà di extent_y.
Camera camera(glm::vec3(0.0f, 0.9945632175300716f/2.0f, 3.0f),
             FOV,
             SCR_WIDTH /SCR_HEIGHT);
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void saveImage(GLFWwindow* w, const char* filepath) {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 4;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}

float getCameraDistance(const float extent_x, const float extent_y, const float extent_z, const glm::mat4 &model) {
    //extent_y dovrebbe avere come coordinate estreme 0 extent_y perché tutti gli oggetti sono sul piano y=0
    glm::vec3 corners[] = {
        glm::vec3(-extent_x/2, 0, -extent_z/2),
        glm::vec3( extent_x/2, 0, -extent_z/2),
        glm::vec3(-extent_x/2,  extent_y, -extent_z/2),
        glm::vec3( extent_x/2,  extent_y, -extent_z/2),
        glm::vec3(-extent_x/2, 0,  extent_z/2),
        glm::vec3( extent_x/2, 0,  extent_z/2),
        glm::vec3(-extent_x/2,  extent_y,  extent_z/2),
        glm::vec3( extent_x/2,  extent_y,  extent_z/2)
    };
    float max_distance = 0.0f;
    for (int i = 0; i < 8; i++) {
        glm::vec3 rotated_corner = glm::vec3(model* glm::vec4(corners[i], 1.0f));
        float distance = glm::length(rotated_corner);
        max_distance = std::max(max_distance, distance);
    }
    return max_distance / glm::tan(glm::radians(FOV) / 2.0f);
}

void randomizeColor(glm::vec3 &randomLightColor) {
    randomLightColor.r = glm::linearRand(0.0f, 1.0f);
    randomLightColor.g = glm::linearRand(0.0f, 1.0f);
    randomLightColor.b = glm::linearRand(0.0f, 1.0f);
}

//IN INPUT path, extent_x, extent_y, extent_z
int main(int argc, char** argv) {
    if (argc  <5 ) {
        std::cerr<<"Missing arguments";
        return -1;
    }

    if (argc > 5 ) {
        std::cerr<<"Too many arguments";
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "glrenderer", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwWindowHint(GLFW_SAMPLES, 16);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //opengl render settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LEQUAL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shader("../src/shaders/model.vs","../src/shaders/model.fs");

    glm::vec3 pointLightsPosition []={
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };


    unsigned int shaderUniformBlockIndexVertex = glGetUniformBlockIndex(shader.ID, "Matrices");
    glUniformBlockBinding(shader.ID, shaderUniformBlockIndexVertex, 0);

    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    float extent_x = std::stof(argv[2]);
    float extent_y = std::stof(argv[3]);
    float extent_z = std::stof(argv[4]);

    auto model = glm::mat4(1.0f);
    float pitch = - 15.0f;
    //nel loop parto aggiungendo 36 gradi
    float yaw = - 36.0f;
    float roll = 0.0f;
    camera.setPosition(glm::vec3(0, extent_y/2.0f, getCameraDistance(extent_x, extent_y, extent_z, model)));

    Model item(argv[1]);
    std::string current_file = argv[1];
    int last_slash = current_file.find_last_of("/");
    int last_dot = current_file.find_last_of(".");
    std::string file_name = current_file.substr(last_slash + 1, last_dot - last_slash -1);
    std::filesystem::create_directory("../resources/output/" + file_name);
    int n_image = 0;

    glm::vec3 dirLightPosition(-2.0f, 4.0f, -1.0f);
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glDisable(GL_FRAMEBUFFER_SRGB);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::mat4(1.0f);
        float  scale_x = glm::linearRand(0.8f, 1.2f);
        float  scale_y = glm::linearRand(0.8f, 1.2f);
        float  scale_z = glm::linearRand(0.8f, 1.2f);

        model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));

        yaw += 36.0f;
        if (yaw >= 359.0f && yaw <= 361.0f) {
            yaw = 0.0f;
            pitch += 15.0f;
        }
        if (pitch >= 99.0f && pitch <= 120.0f) {
            break;
        }
        roll = glm::linearRand(-15.0f, 15.0f);
        const glm::mat4 rotation = glm::eulerAngleXYZ(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
        model *= rotation;

        float camera_height = (extent_y/2.0f) * glm::cos(glm::radians(pitch)) - (extent_z/2) * glm::sin(glm::radians(pitch));
        camera.setPosition(glm::vec3(0, camera_height, getCameraDistance(extent_x, extent_y, extent_z, model)));

        glm::mat4 projection = camera.getPerspectiveMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glm::mat4 view = camera.getViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) , sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        shader.use();
		shader.setFloat("material.shininess", 32.0f);

		// directional light
        dirLightPosition.x = glm::linearRand(-1.0f, 1.0f);
        dirLightPosition.y = glm::linearRand(-1.0f, 1.0f);
        dirLightPosition.z = glm::linearRand(-1.0f, 1.0f);

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

        shader.setMat4("model", model);
        glEnable(GL_FRAMEBUFFER_SRGB);
        item.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::string output_path = "../resources/output/"+ file_name + "/" + file_name + "_" + std::to_string(n_image) + ".png";
        saveImage(window, output_path.c_str());
        n_image ++;
    }

    glfwTerminate();
    return 0;
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
/*
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (format != GL_RGBA){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> texture_faces) {
    unsigned int cubemapID;
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    int width, height, nrChannels;
    unsigned char *data;
    for (unsigned int i= 0; i < texture_faces.size(); i++) {
        data = stbi_load(texture_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr<<"Failed to load cubemap at path " <<texture_faces[i]<<std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return cubemapID;
}
*/