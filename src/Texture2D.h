#pragma once

#include "Texture.h"
#include <string>

/**
 * @brief 2D textúra (PNG, JPG, atď.)
 * 
 * Používa stb_image na načítanie obrázkov
 */
class Texture2D : public Texture
{
private:
    std::string path;
    
public:
    Texture2D(const char* path);
    void BindTexture(int n) override;
};
