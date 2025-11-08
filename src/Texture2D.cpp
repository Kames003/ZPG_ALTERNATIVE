#include "Texture2D.h"

// stb_image implementation
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

Texture2D::Texture2D(const char* path)
{
    this->path = path;
    this->ID = 0;
    this->position = 0;
}

void Texture2D::BindTexture(int n)
{
    glActiveTexture(GL_TEXTURE0 + position);
    
    // Načítaj obrázok pomocou stb_image
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true); // OpenGL má Y os hore, obrázky dole
    
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4); // 4 = RGBA
    
    if (!data) {
        std::cout << "❌ ERROR: Failed to load texture: " << path << std::endl;
        std::cout << "   Reason: " << stbi_failure_reason() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::cout << "✅ Texture loaded: " << path << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    
    // Vytvor OpenGL textúru
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_2D, this->ID);
    
    // Nahraj dáta do OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Nastavenia textúry
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Uvoľni pamäť
    stbi_image_free(data);
}
