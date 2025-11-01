#include "DrawableObject.h"

DrawableObject::DrawableObject(AbstractModel* am, ShaderProgram* sp)
{
    // Uložíme si referencie
    abstractModel = am;
    shaderProgram = sp;

    // Vytvoríme nový transformation composite
    tc = new TransformationComposite();

    // Defaultná farba
    color = glm::vec3(1.0f, 1.0f, 1.0f);
}

DrawableObject::DrawableObject(AbstractModel* am, ShaderProgram* sp, glm::vec3 c)
{
    abstractModel = am;
    shaderProgram = sp;
    tc = new TransformationComposite();
    color = c;  // Nastav farbu z parametra
}

void DrawableObject::drawModel()
{
    // ✅ KRITICKÉ: Aktivuj shader PRED updatom uniformov
    shaderProgram->activateShaderProgram();

    // ✅ KRITICKÉ: Nastav farbu objektu
    shaderProgram->updateUniform("objectColor", color);

    // ✅ KRITICKÉ: Nastav model matrix
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));

    // Vykreslí model (zavolá glDrawArrays/glDrawElements)
    abstractModel->draw();

    // Deaktivuj shader program
    shaderProgram->deactivateShaderProgram();
}

void DrawableObject::rotate(float angle, glm::vec3 axis)
{
    // Pridáme rotáciu do transformation composite
    tc->addTransformation(new TransformationRotate(angle, axis));
}

void DrawableObject::scale(glm::vec3 scale)
{
    // Pridáme škálovanie do transformation composite
    tc->addTransformation(new TransformationScale(scale));
}

void DrawableObject::translate(glm::vec3 translate)
{
    // Pridáme posunutie do transformation composite
    tc->addTransformation(new TransformationTranslate(translate));
}

void DrawableObject::calculateModelMatrix()
{
    // Vypočítame model matrix z všetkých transformácií
    // POZOR: Táto metóda vyprázdni transformation composite!
    tc->calculateTransformations();
}

void DrawableObject::updateModelMatrix()
{
    // Aktivujeme shader
    shaderProgram->activateShaderProgram();

    // Pošleme model matrix do shadera
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));

    // Deaktivujeme shader
    shaderProgram->deactivateShaderProgram();
}

void DrawableObject::updateTexture(const char* variable, int texture)
{
    // Aktivujeme shader
    shaderProgram->activateShaderProgram();

    // Pošleme textúru do shadera
    shaderProgram->updateUniform(variable, texture);

    // Deaktivujeme shader
    shaderProgram->deactivateShaderProgram();
}