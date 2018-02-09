#include "loadShader.h"
#include <fstream>
#include <iostream>
#include <string>

#define strdup _strdup

using namespace std;

/*
Funcio per facilitar la carrega de fitxers.
*/
const char * LoadFromFile(const string filename){
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp) {
		string line, buffer;
		while (getline(fp, line)) {
			buffer.append(line);
			buffer.append("\r\n");
		}
		return strdup(buffer.c_str());
	}
	else {
		cerr << "Error loading shader: " << filename << endl;
		return NULL;
	}
}

GLuint loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;

	/* Creem la shader carregantla del fitxer */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char * shaderSource = LoadFromFile(vertexShaderPath);
	glShaderSource(vertexShader, 1, &shaderSource, NULL);
	GLint status;
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
		cerr << "Compile log: " << infoLog << endl;
		delete[] infoLog;
	}
	free((char *)shaderSource);


	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * shaderSource2 = LoadFromFile(fragmentShaderPath);
	glShaderSource(fragmentShader, 1, &shaderSource2, NULL);


	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
		cerr << "Compile log: " << infoLog << endl;
		delete[] infoLog;
	}
	free((char *)shaderSource2);

	/* Creem el programa unint les dues shaders */
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);


	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		delete[] infoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}