#include "SkyboxModel.h"

SkyboxModel::SkyboxModel()
{
	vbo = new VBO();
	vbo->generateVBO(skycube, sizeof(skycube));

	vao = new VAO();
	vao->generateVAO();

	vao->bind();
	vbo->bind();
	vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	vbo->unbind();
	vao->unbind();
}

void SkyboxModel::draw()
{
	vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vrcholov = 12 trojuholníkov
	vao->unbind();
}
