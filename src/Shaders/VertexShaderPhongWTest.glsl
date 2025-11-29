#version 330

// TEST SHADER - Overenie homogennej zlozky w (BEZ TEXTURY)
// Zadanie: uniform float w = 500 a overit ci vsetko funguje spravne

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec3 worldFragmentPosition;
out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
    float w = 500.0;
    vec4 worldPos = modelMatrix * vec4(v_position, 1.0);
    worldFragmentPosition = worldPos.xyz / worldPos.w;
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position * w, w);
}
