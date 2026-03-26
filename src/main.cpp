#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include<glm/glm.hpp>
#include<glm/ext.hpp>

#include<iostream>

#include "Camera.h"
#include "Shader.h"
#include "primitives/cube.h"

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)
			 , glm::vec3(0.0f, 0.0f, -1.0f)
			 , glm::vec3(0.0f, 1.0f, 0.0f)
			 , static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec2 lastMousePos = glm::vec2(SCR_WIDTH/2,SCR_HEIGHT/2);
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
//per calcolare yaw pitch e roll calcolo l'offset del mouse partendo dal precedente frame al nuovo
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastMousePos.x = xpos;
		lastMousePos.y = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastMousePos.x;
	//al contrario perché y va dal fondo all'inizio
	float yoffset = lastMousePos.y - ypos;

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;
	camera.processMouse(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processScroll(yoffset);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
}

int main() {
	//inizializza glfw, imposta la versione a 3.3, forza utilizzo profilo core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr<< "impossibile creare finestra glfw" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//passiamo a glad la funzione per caricare gli indirizzi delle funzioni opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "impossibile inizializzare GLAD" << std::endl;
		return -1;
	}

	Shader shader("../src/shaders/vertex.vs", "../src/shaders/fragment.fs");
	Shader lightSource("../src/shaders/lightSource.vs", "../src/shaders/lightSource.fs");

	//vertex buffer objects. Dove vengono memorizzati i vertici per poi essere spediti in gpu
	//vertex array object:dove vengono memorizzati gli attributi dei vertici
	//element buffer object: memorizza gli indici dei triangoli
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//collega il VBO con gli ingressi dello shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//dati texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//dati normali
	glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//render loop
	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		process_input(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//comandi rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//cubo arancione
		shader.use();
		shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setFloat("material.shininess", 32.0f);

		glm::vec3 lightColor;
		lightColor.x = glm::sin(glfwGetTime() * 2.0f);
		lightColor.y = glm::sin(glfwGetTime() * 0.7f);
		lightColor.z = glm::sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * 0.5f;
		glm::vec3 ambientColor = lightColor * 0.2f;

		shader.setVec3("light.ambient", ambientColor);
		shader.setVec3("light.diffuse", diffuseColor);
		shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.getPosition());
		auto model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		shader.setMat4("projection", camera.getPerspectiveMatrix());
		shader.setMat4("view", camera.getViewMatrix());
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSource.use();
		glBindVertexArray(lightCubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSource.setMat4("model", model);
		lightSource.setMat4("projection", camera.getPerspectiveMatrix());
		lightSource.setMat4("view", camera.getViewMatrix());
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cambio buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}