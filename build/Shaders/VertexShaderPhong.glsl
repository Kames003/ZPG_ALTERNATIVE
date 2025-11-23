#version 330

// spusti sa pre každy vrchol napr pre bench 108 krat

layout(location=0) in vec3 v_position; // atribut 0 z VAO
layout(location=1) in vec3 v_normal; // atribut 1 z VAO

out vec3 worldFragmentPosition; // output do fragment shadera
out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
    // transformujeme pozíciu do world space
    vec4 position = modelMatrix * vec4(v_position, 1.0);
    worldFragmentPosition = position.xyz / position.w;
    // transformuj normalu
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    // výsledná pozícia na obrazovke
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
}