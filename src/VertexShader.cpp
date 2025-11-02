#include "VertexShader.h"
using namespace std;

GLuint VertexShader::compileShader(const char* filename)
{
    string vertexShaderRead = FileReader::getInstance().readFile(filename);
    shaderCode = vertexShaderRead.c_str();

    // vytvor vertex shader objekt na disku
    ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ID, 1, &shaderCode, NULL); // skopči string shaderCode do shader objektu
    glCompileShader(ID); // skompiluje glsl --> gpu strojový kod

    // debug
    GLint success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        printf("VERTEX SHADER COMPILATION ERROR (%s):\n%s\n", filename, infoLog);
    } else {
        printf("Vertex shader compiled: %s\n", filename);
    }
    return ID; // vrať idčko shadera
}