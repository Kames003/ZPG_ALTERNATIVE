#pragma once

#include "AbstractModel.h"
#include "ShaderProgram.h"
#include "TransformationComposite.h"
#include "LeafRotate.h"
#include "LeafScale.h"
#include "LeafTranslate.h"
#include "Texture.h"
#include <vector>

/**
 * @brief Vykresľovateľný objekt v scéne
 *
 * DrawableObject spája:
 * - AbstractModel (geometria)
 * - ShaderProgram (ako vykresliť)
 * - TransformationComposite (pozícia, rotácia, mierka)
 * - Farba (objectColor uniform)
 */
class DrawableObject
{
private:
	AbstractModel* abstractModel;
	ShaderProgram* shaderProgram;
	TransformationComposite* tc;
	glm::vec3 color;

public:
	bool visible = true;
	std::vector<Texture*> textures;  // Zoznam textúr pre tento objekt

	DrawableObject(AbstractModel* am, ShaderProgram* sp);
	DrawableObject(AbstractModel* am, ShaderProgram* sp, glm::vec3 color);


	void drawModel();

	void rotate(float angle, glm::vec3 axis);
	void scale(glm::vec3 scale);
	void translate(glm::vec3 translate);

	void calculateModelMatrix();
	void updateModelMatrix();
	void updateTexture(const char* variable, int texture);

	void setColor(glm::vec3 c) { color = c; }
	glm::vec3 getColor() { return color; }
	
	// Textúry
	void addTexture(Texture* t) { textures.push_back(t); }

	TransformationComposite* getTransformationComposite() { return tc; }
	AbstractModel* getModel() { return abstractModel; }
	ShaderProgram* getShaderProgram() { return shaderProgram; }
};