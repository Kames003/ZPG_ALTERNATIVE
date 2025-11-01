#include "FragmentShader.h"

GLuint FragmentShader::compileShader(const char* filename)
{
    std::string fragmentShaderRead = FileReader::getInstance().readFile(filename);
    shaderCode = fragmentShaderRead.c_str();

    ID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);

    // ✅ PRIDAJ DEBUG
    GLint success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        printf("❌ FRAGMENT SHADER COMPILATION ERROR (%s):\n%s\n", filename, infoLog);
    } else {
        printf("✅ Fragment shader compiled: %s\n", filename);
    }

    return ID;
}