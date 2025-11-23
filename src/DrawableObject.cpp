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
    
    // Default materiál (bude nahradený z MaterialManagera)
    material = nullptr;
}

DrawableObject::DrawableObject(AbstractModel* am, ShaderProgram* sp, glm::vec3 c)
{
    abstractModel = am;
    shaderProgram = sp;
    tc = new TransformationComposite();
    color = c;  // Nastav farbu z parametra
    material = nullptr;
}


void DrawableObject::drawModel()
{
    shaderProgram->activateShaderProgram();

    // Pošli farbu (pre backward compatibility)
    shaderProgram->updateUniform("objectColor", color);
    
    // Pošli materiál do shaderu (ak existuje)
    if (material != nullptr) {
        material->sendToShader(shaderProgram);
    }
    
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));

    // (volá glDrawArrays)
    abstractModel->draw();

    shaderProgram->deactivateShaderProgram();
}

void DrawableObject::rotate(float angle, glm::vec3 axis)
{
    // Pridáme rotáciu do transformation composite
    tc->addTransformation(new LeafRotate(angle, axis));
}

void DrawableObject::scale(glm::vec3 scale)
{
    // Pridáme škálovanie do transformation composite
    tc->addTransformation(new LeafScale(scale));
}

void DrawableObject::translate(glm::vec3 translate)
{
    // Pridáme posunutie do transformation composite
    tc->addTransformation(new LeafTranslate(translate));
}

void DrawableObject::customW(float w)
{
    // Legacy metóda - použi radšej customMatrix()
    // Jednotková matica s [3][3] = w namiesto 1.0
    tc->addTransformation(new LeafCustomW(w));
}

void DrawableObject::customMatrix(const glm::mat4& m)
{
    // Všeobecná custom matica - môžeš pridať akúkoľvek transformáciu
    tc->addTransformation(new LeafMatrix(m));
}

void DrawableObject::calculateModelMatrix()
{
    // Vypočítame model matrix z všetkých transformácií
    // POZOR: Táto metóda vyprázdni transformation composite! ale vdaka tomu sme schopny realizovať našu animacie
    tc->calculateTransformations();
}


void DrawableObject::updateModelMatrix()
{
    shaderProgram->activateShaderProgram();
    shaderProgram->updateUniform("modelMatrix",
        glm::value_ptr(tc->getResultMatrix()));
    shaderProgram->deactivateShaderProgram();
}


void DrawableObject::updateTexture(const char* variable, int texture)
{
    shaderProgram->activateShaderProgram();
    shaderProgram->updateUniform(variable, texture);
    shaderProgram->deactivateShaderProgram();
}