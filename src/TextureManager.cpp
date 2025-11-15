#include "TextureManager.h"

void TextureManager::addTexture(Texture* t)
{
    // Priradí pozíciu na základe počtu existujúcich textúr
    t->position = textures.size();
    textures.push_back(t);
    
    // Bindne textúru (načíta obrázok a vytvorí OpenGL textúru)
    t->BindTexture(t->position);
    
    printf("  Texture added at position %d\n", t->position);
}

Texture* TextureManager::getTexture(int index)
{
    if (index < 0 || index >= textures.size())
    {
        printf("ERROR: Texture %d does not exist.\n", index);
        exit(EXIT_FAILURE);
    }
    
    return this->textures[index];
}
