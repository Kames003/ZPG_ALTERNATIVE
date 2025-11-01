#include "VertexShader.h"

GLuint VertexShader::compileShader(const char* filename)
{
    std::string vertexShaderRead = FileReader::getInstance().readFile(filename);
    shaderCode = vertexShaderRead.c_str();

    ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);

    // ✅ PRIDAJ DEBUG
    GLint success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        printf("❌ VERTEX SHADER COMPILATION ERROR (%s):\n%s\n", filename, infoLog);
    } else {
        printf("✅ Vertex shader compiled: %s\n", filename);
    }

    return ID;
}