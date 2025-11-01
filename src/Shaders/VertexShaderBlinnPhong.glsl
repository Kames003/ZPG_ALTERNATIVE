#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec4 worldPosition;
out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
    worldPosition = modelMatrix * vec4(v_position, 1.0);
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
}