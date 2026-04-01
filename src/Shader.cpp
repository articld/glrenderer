#include "Shader.h"

void log_shader_compilation(const unsigned int shader) {
	int success;
	char infolog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cerr << "ERRORE: COMPILAZIONE SHADER FALLITA\n" << infolog << std::endl;
	}
}

void log_program_compilation(unsigned int program) {
	int success;
	char infolog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infolog);
		std::cerr << "ERRORE: COMPILAZIONE PROGRAMMA FALLITA\n" << infolog << std::endl;
	}
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERRORE: File shader non letto"<<std::endl;
		std::cerr << fragmentPath <<std::endl << vertexPath<<std::endl;
		std::cerr << std::filesystem::current_path() <<std::endl;
		throw;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	log_shader_compilation(vertex);

	fragment= glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	log_shader_compilation(fragment);

	//collega gli output di uno shader con gli input dell'altro. Si chiama durante il rendering
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	log_program_compilation(ID);
	//possono essere cancellati appena vengono linkati dal programma
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 vector) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector[0], vector[1], vector[2]);
}

void Shader::setVec3(const std::string& name, const float x, const float y, const float z) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
