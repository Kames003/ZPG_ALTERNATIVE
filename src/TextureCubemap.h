#pragma once

#include "Texture.h"
#include <string>

/**
 * @brief Cubemap textúra pre skybox
 * 
 * Skladá sa zo 6 obrázkov: right, left, top, bottom, front, back = viac vo folder textures
 */
class TextureCubemap : public Texture
{
private:
    std::string right;
    std::string left;
    std::string top;
    std::string bottom;
    std::string front;
    std::string back;
    
public:
    TextureCubemap(const char* right, const char* left, const char* top, 
                   const char* bottom, const char* front, const char* back);
    void BindTexture(int n) override;
};
