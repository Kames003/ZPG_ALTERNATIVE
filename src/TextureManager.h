#pragma once

#include "Texture2D.h"
#include "TextureCubemap.h"
#include <vector>

/**
 * @brief Správca všetkých textúr v scéne
 * 
 * Centrálne ukladá všetky textúry a priraďuje im pozície
 */
class TextureManager
{
private:
    std::vector<Texture*> textures;
    
public:
    ~TextureManager() {
        for (Texture* t : textures) {
            delete t;
        }
        textures.clear();
    }
    
    /**
     * @brief Pridá textúru do managera a bindne ju
     * @param t Ukazovateľ na textúru
     */
    void addTexture(Texture* t);
    
    /**
     * @brief Získa textúru podľa indexu
     * @param index Index textúry v zozname
     * @return Ukazovateľ na textúru
     */
    Texture* getTexture(int index);
    
    int getTextureCount() const { return textures.size(); }
};
