#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * @brief Abstraktná trieda pre textúry
 * 
 * Definuje rozhranie pre všetky typy textúr (2D, Cubemap, atď.)
 */
class Texture
{
protected:
    GLuint ID;
    
public:
    int position;  // Pozícia textúry v TextureManager (0, 1, 2, ...)
    
    virtual ~Texture() {
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
    }
    
    /**
     * @brief Bindne textúru na danú texture unit
     * @param n Texture unit (GL_TEXTURE0 + position)
     */
    virtual void BindTexture(int n) = 0;
    
    GLuint getID() const { return ID; }
};
