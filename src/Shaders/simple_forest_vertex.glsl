#version 330

// INPUT z VBO
layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

// OUTPUT do fragment shadera
out vec4 worldPosition;
out vec3 worldNormal;

// UNIFORMS
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    // Vypočítaj world pozíciu
    worldPosition = modelMatrix * vec4(v_position, 1.0);

    // Transformuj normálu do world space
    worldNormal = normalize(mat3(transpose(inverse(modelMatrix))) * v_normal);

    // Finálna pozícia na obrazovke
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}