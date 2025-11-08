#pragma once

#include <glm/vec3.hpp>
#include <string>
#include "ShaderProgram.h"

/**
 * @brief Material class pre Phong/Blinn-Phong osvetľovací model
 * 
 * Mapovanie MTL vlastností:
 * - ra (ambient)  ← Ka (ambient coefficient)
 * - rd (diffuse)  ← Kd (diffuse coefficient)  
 * - rs (specular) ← Ks (specular coefficient)
 * - h (shininess) ← Ns (shininess exponent)
 */
class Material {
private:
    glm::vec3 ra;  // Ambient reflection coefficient (Ka)
    glm::vec3 rd;  // Diffuse reflection coefficient (Kd)
    glm::vec3 rs;  // Specular reflection coefficient (Ks)
    float h;       // Shininess exponent (Ns) w
    
    std::string name;
    
public:
    // Konštruktor s default hodnotami
    Material()
        : ra(0.2f, 0.2f, 0.2f)
        , rd(0.8f, 0.8f, 0.8f)
        , rs(1.0f, 1.0f, 1.0f)
        , h(32.0f)
        , name("default")
    {}
    
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, 
             const glm::vec3& specular, float shininess, 
             const std::string& materialName = "custom")
        : ra(ambient)
        , rd(diffuse)
        , rs(specular)
        , h(shininess)
        , name(materialName)
    {}
    
    // Getters
    glm::vec3 getAmbient() const { return ra; }
    glm::vec3 getDiffuse() const { return rd; }
    glm::vec3 getSpecular() const { return rs; }
    float getShininess() const { return h; }
    std::string getName() const { return name; }
    
    // Setters
    void setAmbient(const glm::vec3& ambient) { ra = ambient; }
    void setDiffuse(const glm::vec3& diffuse) { rd = diffuse; }
    void setSpecular(const glm::vec3& specular) { rs = specular; }
    void setShininess(float shininess) { h = shininess; }
    void setName(const std::string& materialName) { name = materialName; }
    
    /**
     * @brief Načíta materiál z MTL hodnôt
     */
    void loadFromMTL(const glm::vec3& Ka, const glm::vec3& Kd, 
                     const glm::vec3& Ks, float Ns) {
        ra = Ka;
        rd = Kd;
        rs = Ks;
        h = Ns;
    }
    
    /**
     * @brief Pošle materiálové vlastnosti do shaderu
     * @param shaderProgram Ukazovateľ na ShaderProgram objekt
     */
    void sendToShader(ShaderProgram* shaderProgram) const {
        if (shaderProgram == nullptr) return;
        
        shaderProgram->updateUniform("material.ambient", ra);
        shaderProgram->updateUniform("material.diffuse", rd);
        shaderProgram->updateUniform("material.specular", rs);
        shaderProgram->updateUniform("material.shininess", h);
    }
    
    /**
     * @brief Debug výpis materiálu
     */
    void print() const {
        printf("═══════════════════════════════\n");
        printf("Material: %s\n", name.c_str());
        printf("───────────────────────────────\n");
        printf("Ambient  (ra): (%.3f, %.3f, %.3f)\n", ra.x, ra.y, ra.z);
        printf("Diffuse  (rd): (%.3f, %.3f, %.3f)\n", rd.x, rd.y, rd.z);
        printf("Specular (rs): (%.3f, %.3f, %.3f)\n", rs.x, rs.y, rs.z);
        printf("Shininess (h): %.1f\n", h);
        printf("═══════════════════════════════\n");
    }
    
    // Predefined materials
    static Material Gold() {
        return Material(
            glm::vec3(0.24725f, 0.1995f, 0.0745f),
            glm::vec3(0.75164f, 0.60648f, 0.22648f),
            glm::vec3(0.628281f, 0.555802f, 0.366065f),
            51.2f, "Gold"
        );
    }
    
    static Material Silver() {
        return Material(
            glm::vec3(0.19225f, 0.19225f, 0.19225f),
            glm::vec3(0.50754f, 0.50754f, 0.50754f),
            glm::vec3(0.508273f, 0.508273f, 0.508273f),
            51.2f, "Silver"
        );
    }
    
    static Material Bronze() {
        return Material(
            glm::vec3(0.2125f, 0.1275f, 0.054f),
            glm::vec3(0.714f, 0.4284f, 0.18144f),
            glm::vec3(0.393548f, 0.271906f, 0.166721f),
            25.6f, "Bronze"
        );
    }
    
    static Material Ruby() {
        return Material(
            glm::vec3(0.1745f, 0.01175f, 0.01175f),
            glm::vec3(0.61424f, 0.04136f, 0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            76.8f, "Ruby"
        );
    }
    
    static Material Emerald() {
        return Material(
            glm::vec3(0.0215f, 0.1745f, 0.0215f),
            glm::vec3(0.07568f, 0.61424f, 0.07568f),
            glm::vec3(0.633f, 0.727811f, 0.633f),
            76.8f, "Emerald"
        );
    }
};
