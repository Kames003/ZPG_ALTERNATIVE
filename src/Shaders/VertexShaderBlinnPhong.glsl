#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec3 worldFragmentPosition;  // ✅ vec3 namiesto vec4!
out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
    vec4 position = modelMatrix * vec4(v_position, 1.0);
    worldFragmentPosition = position.xyz / position.w;  // ✅ Konverzia na vec3
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
}