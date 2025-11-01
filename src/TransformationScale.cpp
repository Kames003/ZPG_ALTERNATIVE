#include "TransformationScale.h"

TransformationScale::TransformationScale(glm::vec3 scale)
{
	// Vytvorenie škálovacej matice pomocou GLM
	matrix = glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 TransformationScale::resultMatrix()
{
	return matrix;
}