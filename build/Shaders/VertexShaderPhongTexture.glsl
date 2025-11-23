#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_texture;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldFragmentPosition;
out vec3 worldNormal;
out vec2 textureCoords;

void main () {
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
    vec4 position = modelMatrix * vec4(v_position, 1.0f);
    worldFragmentPosition = position.xyz / position.w;
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    textureCoords = v_texture;
}
