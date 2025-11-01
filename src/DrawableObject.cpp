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

// ========================================
// ✅ SPRÁVNE: glUseProgram PRED glUniform a glDrawArrays
// ========================================
void DrawableObject::drawModel()
{
    // ✅ KROK 1: Aktivuj shader PRED všetkými operáciami
    shaderProgram->activateShaderProgram();

    // ✅ KROK 2: Nastav uniformy (volá glUniform* interně)
    shaderProgram->updateUniform("objectColor", color);
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));

    // ✅ KROK 3: Vykresli model (volá glDrawArrays)
    abstractModel->draw();

    // ✅ KROK 4: Deaktivuj shader PO všetkých operáciách
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

// ========================================
// ✅ SPRÁVNE: glUseProgram okolo glUniform
// ========================================
void DrawableObject::updateModelMatrix()
{
    // ✅ Aktivujeme shader
    shaderProgram->activateShaderProgram();

    // ✅ Pošleme model matrix do shadera
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));

    // ✅ Deaktivujeme shader
    shaderProgram->deactivateShaderProgram();
}

// ========================================
// ✅ SPRÁVNE: glUseProgram okolo glUniform
// ========================================
void DrawableObject::updateTexture(const char* variable, int texture)
{
    // ✅ Aktivujeme shader
    shaderProgram->activateShaderProgram();

    // ✅ Pošleme textúru do shadera
    shaderProgram->updateUniform(variable, texture);

    // ✅ Deaktivujeme shader
    shaderProgram->deactivateShaderProgram();
}