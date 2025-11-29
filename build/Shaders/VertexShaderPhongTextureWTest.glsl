#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_texture;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float customW;

out vec3 worldFragmentPosition;
out vec3 worldNormal;
out vec2 textureCoords;

void main () {

    // vec4(pos * w, w) – after the perspective divide, the w component gets reduced
    vec4 worldPos = modelMatrix * vec4(v_position * customW, customW);
    worldFragmentPosition = worldPos.xyz / worldPos.w;
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);
    textureCoords = v_texture;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
