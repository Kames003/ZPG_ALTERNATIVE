#include "ShaderProgram.h"

void ShaderProgram::CheckErrors(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	GLint status;
	glGetProgramiv(ID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(ID, infoLogLength, NULL, strInfoLog);
		printf("Shaders: %s | %s\n", vertexShaderFile, fragmentShaderFile);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
		exit(EXIT_FAILURE);
	}
}

ShaderProgram::ShaderProgram(Camera* c)
{
	this->camera = c;
}

void ShaderProgram::compileShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	Shader* vertexShader = new VertexShader();
	Shader* fragmentShader = new FragmentShader();

	ID = glCreateProgram();
	glAttachShader(ID, fragmentShader->compileShader(fragmentShaderFile));
	glAttachShader(ID, vertexShader->compileShader(vertexShaderFile));
	glLinkProgram(ID);

	CheckErrors(vertexShaderFile, fragmentShaderFile);
}

void ShaderProgram::activateShaderProgram()
{
	glUseProgram(ID);
}

void ShaderProgram::deactivateShaderProgram()
{
	glUseProgram(0);
}

// ========================================
// ✅ VŠECHNY updateUniform metódy už NEVOLAJÍ activate/deactivate
// Volá sa to zvonku v drawModel() alebo update()
// ========================================

void ShaderProgram::updateUniform(const char* variable, const GLfloat* value)
{
	GLint location = glGetUniformLocation(ID, variable);

	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}
}

void ShaderProgram::updateUniform(const char* variable, glm::vec3 value)
{
	GLint location = glGetUniformLocation(ID, variable);

	if (location >= 0)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
}

void ShaderProgram::updateUniform(const char* variable, float value)
{
	GLint location = glGetUniformLocation(ID, variable);

	if (location >= 0)
	{
		glUniform1f(location, value);
	}
}

void ShaderProgram::updateUniform(const char* variable, int value)
{
	GLint location = glGetUniformLocation(ID, variable);

	if (location >= 0)
	{
		glUniform1i(location, value);
	}
}

void ShaderProgram::updateUniform(const char* variable, bool value)
{
	// Bool sa posiela ako int (0 alebo 1)
	updateUniform(variable, value ? 1 : 0);
}

void ShaderProgram::updateUniform(const char* variable, glm::vec4 value)
{
	GLint location = glGetUniformLocation(ID, variable);
	if (location >= 0)
	{
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}

// ========================================
// ✅ OPRAVENÉ: Observer pattern update
// ========================================
void ShaderProgram::update(int message)
{
	if (message == VIEWMATRIX)
	{
		activateShaderProgram();  // ✅ Aktivuj PRED updatemi
		updateUniform("viewMatrix", glm::value_ptr(camera->getViewMatrix()));
		updateUniform("viewPosition", camera->getCameraPosition());
		updateUniform("cameraDirection", camera->getCameraDirection());
		deactivateShaderProgram();  // ✅ Deaktivuj PO updatech

		return;
	}

	if (message == PROJECTIONMATRIX)
	{
		activateShaderProgram();  // ✅ Aktivuj PRED updatemi
		updateUniform("projectionMatrix", glm::value_ptr(camera->getProjectionMatrix()));
		deactivateShaderProgram();  // ✅ Deaktivuj PO updatech

		return;
	}
}