#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Observer.h"
#include "Camera.h"
#include "VertexShader.h"
#include "FragmentShader.h"


class ShaderProgram : public Observer
{
private:
	GLuint ID; // shaderprogram + vertexshader
	Camera* camera;
	void CheckErrors(const char* vertexShaderFile, const char* fragmentShaderFile);

public:
	ShaderProgram(Camera* c);

	// Skompiluje a zlinkuje vertex + fragment shader
	void compileShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);

	// state machine fun
	void activateShaderProgram(); // glUseProgram(ID)
	void deactivateShaderProgram();// glUseProgram(0)


	void updateUniform(const char* variable, const GLfloat* value);
	void updateUniform(const char* variable, glm::vec3 value);
	void updateUniform(const char* variable, float value);
	void updateUniform(const char* variable, int value);
	void updateUniform(const char* variable, bool value);
	void updateUniform(const char* variable, glm::vec4 value);

	// observeros
	void notify(int message);

};