#include "TextureCubemap.h"
#include "external/stb_image.h"

TextureCubemap::TextureCubemap(const char* right, const char* left, const char* top, 
                               const char* bottom, const char* front, const char* back)
{
    this->position = 0;
    this->right = right;
    this->left = left;
    this->top = top;
    this->bottom = bottom;
    this->front = front;
    this->back = back;
    this->ID = 0;
}

void TextureCubemap::BindTexture(int n)
{
    glActiveTexture(GL_TEXTURE0 + position);
    
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
    
    // Pole ciest k obrázkom v správnom poradí
    std::string faces[6] = {
        right,   // GL_TEXTURE_CUBE_MAP_POSITIVE_X
        left,    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
        top,     // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
        bottom,  // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        front,   // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
        back     // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    
    stbi_set_flip_vertically_on_load(false); // Cubemap nepotrebuje flip
    
    for (int i = 0; i < 6; i++)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 3); // 3 = RGB
        
        if (!data) {
            std::cout << "❌ ERROR: Failed to load cubemap texture: " << faces[i] << std::endl;
            std::cout << "   Reason: " << stbi_failure_reason() << std::endl;
            exit(EXIT_FAILURE);
        }
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                     0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    }
    
    std::cout << "✅ Cubemap loaded successfully (6 faces)" << std::endl;
    
    // Nastavenia cubemap textúry
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
