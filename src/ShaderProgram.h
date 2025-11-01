#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Observer.h"
#include "Camera.h"
#include "VertexShader.h"
#include "FragmentShader.h"

/**
 * @brief Shader program - kombinuje vertex a fragment shader
 *
 * ShaderProgram je Observer, ktorý reaguje na zmeny v Camera (Subject).
 * Keď sa zmení view matrix alebo projection matrix, automaticky sa updatujú uniformy.
 */
class ShaderProgram : public Observer
{
private:
	/**
	 * @brief OpenGL ID shader programu
	 */
	GLuint ID;

	/**
	 * @brief Kamera pripojená k tomuto shaderu
	 *
	 * Shader potrebuje poznať kameru aby mohol updatovať view a projection matrix
	 */
	Camera* camera;

	/**
	 * @brief Kontrola chýb pri linkovaní shader programu
	 */
	void CheckErrors(const char* vertexShaderFile, const char* fragmentShaderFile);

public:
	/**
	 * @brief Konštruktor
	 *
	 * @param c Camera - automaticky sa pripojí ako observer
	 */
	ShaderProgram(Camera* c);

	/**
	 * @brief Skompiluje a zlinkuje shader program
	 *
	 * @param vertexShaderFile Cesta k vertex shader súboru
	 * @param fragmentShaderFile Cesta k fragment shader súboru
	 */
	void compileShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);

	/**
	 * @brief Aktivuje shader program (glUseProgram)
	 */
	void activateShaderProgram();

	/**
	 * @brief Deaktivuje shader program (glUseProgram(0))
	 */
	void deactivateShaderProgram();

	/**
	 * @brief Updatuje uniform premennú - mat4 (matica)
	 *
	 * @param variable Názov uniform premennej v shaderi
	 * @param value Pointer na GLfloat array (matica)
	 */
	void updateUniform(const char* variable, const GLfloat* value);

	/**
	 * @brief Updatuje uniform premennú - vec3
	 *
	 * @param variable Názov uniform premennej v shaderi
	 * @param value glm::vec3 hodnota
	 */
	void updateUniform(const char* variable, glm::vec3 value);

	/**
	 * @brief Updatuje uniform premennú - float
	 *
	 * @param variable Názov uniform premennej v shaderi
	 * @param value float hodnota
	 */
	void updateUniform(const char* variable, float value);

	/**
	 * @brief Updatuje uniform premennú - int
	 *
	 * @param variable Názov uniform premennej v shaderi
	 * @param value int hodnota
	 */
	void updateUniform(const char* variable, int value);

	/**
	 * @brief Updatuje uniform premennú - bool
	 *
	 * @param variable Názov uniform premennej v shaderi
	 * @param value bool hodnota (konvertuje sa na int)
	 */
	void updateUniform(const char* variable, bool value);

	/**
	 * @brief Observer pattern - reakcia na zmeny v Camera
	 *
	 * @param message VIEWMATRIX alebo PROJECTIONMATRIX
	 *
	 * Keď sa zmení view matrix:
	 * - Updatuje viewMatrix uniform
	 * - Updatuje cameraPosition uniform
	 * - Updatuje cameraDirection uniform
	 *
	 * Keď sa zmení projection matrix:
	 * - Updatuje projectionMatrix uniform
	 */
	void update(int message);



};