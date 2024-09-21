#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath = "", const char* fragmentPath = "") {
		// Import shader code from filepaths
		// ---------------------------------
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			// Read files buffer content into streams
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Close files
			vShaderFile.close();
			fShaderFile.close();

			// Convert stream to string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR READING SHADER FILES\n" << std::endl;
			std::cout << vertexPath << std::endl;
			std::cout << fragmentPath << std::endl;
			return;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		// Compile shaders
		// ---------------
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Check for compilation errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR COMPILING VERTEX SHADER\n" << infoLog << std::endl;
		}

		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Check for compilation errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR COMPILING FRAGMENT SHADER\n" << infoLog << std::endl;
		}


		// Link Shader Program
		// -------------------
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// Check for linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR LINKING SHADER PROGRAM\n" << infoLog << std::endl;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use(){
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const {
		int uniformLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(uniformLocation, value);
	}

	void setFloat(const std::string& name, float value) const {
		int uniformLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1f(uniformLocation, value);
	}

	void setInt(const std::string& name, int value) const {
		int uniformLocation = glGetUniformLocation(ID, name.c_str());
		glUniform1i(uniformLocation, value);
	}

	void setVec3(const std::string& name, glm::vec3 value) const {
		int uniformLocation = glGetUniformLocation(ID, name.c_str());
		glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
	}

	void setMat4(const std::string& name, glm::mat4 value) const {
		int uniformLocation = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}
};

