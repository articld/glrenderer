#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include"Shader.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	//inizializza glfw, imposta la versione a 3.3, forza utilizzo profilo core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr<< "impossibile creare finestra glfw" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//passiamo a glad la funzione per caricare gli indirizzi delle funzioni opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "impossibile inizializzare GLAD" << std::endl;
		return -1;
	}

	Shader shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

	float vertices[] = {
		//� un rettangolo, uso triangoli ma posso omettere i vertici gi� esistenti
		//vertici		   //colori
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	//vertex buffer objects. Dove vengono memorizzati i vertici per poi essere spediti in gpu
	//vertex array object:dove vengono memorizzati gli attributi dei vertici
	//element buffer object: memorizza gli indici dei triangoli
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//collega il VBO con gli ingressi dello shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//render loop
	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		//input
		process_input(window);

		//comandi rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		//cambio buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Scollega VBO e VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glfwTerminate();
	return 0;
}
