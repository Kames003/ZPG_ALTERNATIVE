#pragma once

#include "Material.h"
#include "MaterialManager.h"
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

/**
 * @brief Loader pre MTL súbory (Material Template Library)
 * 
 * MTL súbory obsahujú materiálové definície pre OBJ modely.
 * Tento loader načíta všetky materiály a pridá ich do MaterialManagera.
 * 
 * OOP Design: Separátna trieda pre loading logic (Single Responsibility Principle)
 */
class MTLLoader
{
public:
    /**
     * @brief Načíta MTL súbor a pridá materiály do managera
     * @param mtlPath Cesta k MTL súboru
     * @param manager MaterialManager kam pridať materiály
     * @return Počet načítaných materiálov
     */
    static int loadMTL(const std::string& mtlPath, MaterialManager* manager)
    {
        printf("📄 [MTLLoader] Načítavam: %s\n", mtlPath.c_str());
        
        FILE* file = fopen(mtlPath.c_str(), "r");
        if (!file) {
            printf("⚠️  [MTLLoader] Súbor sa nepodarilo otvoriť: %s\n", mtlPath.c_str());
            return 0;
        }
        
        Material* currentMaterial = nullptr;
        char line[256];
        int materialsLoaded = 0;
        bool hasKd = false;  // Sleduj či má Kd definované

        while (fgets(line, sizeof(line), file))
        {
            // Skip comments and empty lines
            if (line[0] == '#' || line[0] == '\n') {
                continue;
            }

            // Nový materiál: "newmtl <name>"
            if (strncmp(line, "newmtl ", 7) == 0)
            {
                // Ak predchádzajúci materiál nemal Kd, nastav default (pre textúry)
                if (currentMaterial != nullptr && !hasKd) {
                    currentMaterial->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
                    printf("      ℹ️  Kd nebolo definované, nastavené na (1,1,1)\n");
                }

                char name[128];
                sscanf(line, "newmtl %s", name);

                currentMaterial = new Material();
                currentMaterial->setName(std::string(name));
                manager->addMaterial(currentMaterial);
                materialsLoaded++;
                hasKd = false;  // Reset pre nový materiál

                printf("   📦 Materiál: %s\n", name);
            }
            // Ambient: "Ka r g b"
            else if (strncmp(line, "Ka ", 3) == 0 && currentMaterial)
            {
                float r, g, b;
                sscanf(line, "Ka %f %f %f", &r, &g, &b);
                currentMaterial->setAmbient(glm::vec3(r, g, b));
            }
            // Diffuse: "Kd r g b"
            else if (strncmp(line, "Kd ", 3) == 0 && currentMaterial)
            {
                float r, g, b;
                sscanf(line, "Kd %f %f %f", &r, &g, &b);
                currentMaterial->setDiffuse(glm::vec3(r, g, b));
                hasKd = true;
            }
            // Specular: "Ks r g b"
            else if (strncmp(line, "Ks ", 3) == 0 && currentMaterial)
            {
                float r, g, b;
                sscanf(line, "Ks %f %f %f", &r, &g, &b);
                currentMaterial->setSpecular(glm::vec3(r, g, b));
            }
            // Shininess: "Ns <value>"
            else if (strncmp(line, "Ns ", 3) == 0 && currentMaterial)
            {
                float ns;
                sscanf(line, "Ns %f", &ns);
                currentMaterial->setShininess(ns);
            }
        }

        // Skontroluj posledný materiál
        if (currentMaterial != nullptr && !hasKd) {
            currentMaterial->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
            printf("      ℹ️  Kd nebolo definované, nastavené na (1,1,1)\n");
        }

        fclose(file);
        printf("✅ [MTLLoader] Načítaných %d materiálov\n\n", materialsLoaded);

        return materialsLoaded;
    }
    
    /**
     * @brief Automaticky nájde MTL súbor pre daný OBJ model
     * @param objPath Cesta k OBJ súboru (napr. "models/lamp.obj")
     * @return Cesta k MTL súboru (napr. "models/lamp.mtl")
     */
    static std::string getMTLPathFromOBJ(const std::string& objPath)
    {
        // Nahraď .obj za .mtl
        std::string mtlPath = objPath;
        size_t lastDot = mtlPath.find_last_of(".");
        
        if (lastDot != std::string::npos) {
            mtlPath = mtlPath.substr(0, lastDot) + ".mtl";
        } else {
            mtlPath += ".mtl";
        }
        
        return mtlPath;
    }
    
    /**
     * @brief Skontroluje, či MTL súbor existuje
     * @param mtlPath Cesta k MTL súboru
     * @return true ak existuje, inak false
     */
    static bool mtlFileExists(const std::string& mtlPath)
    {
        FILE* file = fopen(mtlPath.c_str(), "r");
        if (file) {
            fclose(file);
            return true;
        }
        return false;
    }
};
