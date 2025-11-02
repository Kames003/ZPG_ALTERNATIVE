#include "ShaderProgram.h"

void ShaderProgram::CheckErrors(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	// nemec lms check
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
	this->camera = c; // neskôr potrebujeme ziskat cez to data kamery
}

void ShaderProgram::compileShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	// vytvor objekty pre vert a frag
	Shader* vertexShader = new VertexShader();
	Shader* fragmentShader = new FragmentShader();

	ID = glCreateProgram(); // create shaderprogram na gpučku

	// pripoj fragmentShader
	glAttachShader(ID, fragmentShader->compileShader(fragmentShaderFile));
	// pripoj vertexShader
	glAttachShader(ID, vertexShader->compileShader(vertexShaderFile));
	// zlinkuj tieto dva ktoré sú v shaderprograme
	// vertexS output + fragS input
	glLinkProgram(ID);

	// call debug machiny z lms
	CheckErrors(vertexShaderFile, fragmentShaderFile);
}


// state machine fun
// jeden moment jeden aktivny shader program
// všetky OpenGl operácie ( glUniform.. ) sa vztahuju a aktivnemu programu
void ShaderProgram::activateShaderProgram()
{
	glUseProgram(ID);
}

// !active :)
void ShaderProgram::deactivateShaderProgram()
{
	glUseProgram(0);
}


void ShaderProgram::updateUniform(const char* variable, const GLfloat* value)
{
	// najdi pozíciu uniformu v sh
	GLint location = glGetUniformLocation(ID, variable);

	if (location >= 0) // exists ?
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}
}

void ShaderProgram::updateUniform(const char* variable, glm::vec3 value)
{
	GLint location = glGetUniformLocation(ID, variable); // toto vracia adresu uniformu v shadery

	if (location >= 0) // exists ?
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


void ShaderProgram::notify(int message)
{
	if (message == VIEWMATRIX) // reakcia a
	{
		activateShaderProgram();
		updateUniform("viewMatrix", glm::value_ptr(camera->getViewMatrix()));
		updateUniform("viewPosition", camera->getCameraPosition());
		updateUniform("cameraDirection", camera->getCameraDirection());
		deactivateShaderProgram();

		return;
	}

	if (message == PROJECTIONMATRIX) // reakcia b
	{
		activateShaderProgram();
		updateUniform("projectionMatrix", glm::value_ptr(camera->getProjectionMatrix()));
		deactivateShaderProgram();

		return;
	}
}