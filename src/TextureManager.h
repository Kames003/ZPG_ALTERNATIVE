#pragma once

#include "Texture2D.h"
#include "TextureCubemap.h"
#include <vector>


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

    void addTexture(Texture* t);
    Texture* getTexture(int index);
    
    int getTextureCount() const { return textures.size(); }
};
