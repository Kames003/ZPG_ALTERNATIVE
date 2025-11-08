#pragma once

#include "Material.h"
#include <vector>
#include <string>
#include <cstdio>

/**
 * @brief Správca všetkých materiálov v scéne
 *
 * Centrálne ukladá materiály a umožňuje:
 * - Pridávanie vlastných materiálov
 * - Získavanie materiálov podľa indexu alebo mena
 * - Správu predefinovaných materiálov (Gold, Silver, atď.)
 *
 * POZNÁMKA: Pre načítanie materiálov z MTL súboru použite MTLLoader::loadMTL()
 */
class MaterialManager
{
private:
    std::vector<Material*> materials;
    
public:
    MaterialManager() {
        // Pridaj default materiál
        addMaterial(new Material());
    }
    
    ~MaterialManager() {
        for (Material* m : materials) {
            delete m;
        }
        materials.clear();
    }
    
    /**
     * @brief Pridá materiál do managera
     * @param m Ukazovateľ na materiál
     */
    void addMaterial(Material* m) {
        materials.push_back(m);
        printf("✅ Material '%s' pridaný (index: %d)\n", 
               m->getName().c_str(), (int)materials.size() - 1);
    }
    
    /**
     * @brief Získa materiál podľa indexu
     * @param index Index materiálu v zozname
     * @return Ukazovateľ na materiál
     */
    Material* getMaterial(int index) {
        if (index < 0 || index >= (int)materials.size()) {
            printf("⚠️  Materiál s indexom %d neexistuje! Vraciam default.\n", index);
            return materials[0]; // default material
        }
        return materials[index];
    }
    
    /**
     * @brief Získa materiál podľa mena
     * @param name Meno materiálu
     * @return Ukazovateľ na materiál (alebo default ak nenájde)
     */
    Material* getMaterialByName(const std::string& name) {
        for (Material* m : materials) {
            if (m->getName() == name) {
                return m;
            }
        }
        printf("⚠️  Materiál '%s' nenájdený! Vraciam default.\n", name.c_str());
        return materials[0]; // default material
    }
    
    /**
     * @brief Vytvorí a pridá predefinovaný materiál
     * @param type Typ materiálu (Gold, Silver, Bronze, Ruby, Emerald)
     * @return Index pridaného materiálu
     */
    int addPredefinedMaterial(const std::string& type) {
        Material* mat = nullptr;
        
        if (type == "Gold") mat = new Material(Material::Gold());
        else if (type == "Silver") mat = new Material(Material::Silver());
        else if (type == "Bronze") mat = new Material(Material::Bronze());
        else if (type == "Ruby") mat = new Material(Material::Ruby());
        else if (type == "Emerald") mat = new Material(Material::Emerald());
        else {
            printf("⚠️  Neznámy typ materiálu: %s\n", type.c_str());
            return 0;
        }
        
        addMaterial(mat);
        return materials.size() - 1;
    }
    
    /**
     * @brief Vypíše všetky materiály (debug)
     */
    void printAllMaterials() const {
        printf("\n📋 MATERIAL MANAGER - Zoznam materiálov (%d)\n", getMaterialCount());
        printf("═══════════════════════════════════════════\n");
        for (int i = 0; i < (int)materials.size(); i++) {
            printf("[%d] %s\n", i, materials[i]->getName().c_str());
        }
        printf("═══════════════════════════════════════════\n\n");
    }
    
    /**
     * @brief Počet materiálov v manageri
     */
    int getMaterialCount() const { 
        return static_cast<int>(materials.size()); 
    }
};
