#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_texture;
layout(location=3) in vec3 v_tangent;
layout(location=4) in vec3 v_bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldFragmentPosition;
out vec3 worldNormal;
out vec2 textureCoords;
out mat3 tbn;

void main () {
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
    vec4 position = modelMatrix * vec4(v_position, 1.0f);
    worldFragmentPosition = position.xyz / position.w;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalMatrix * v_normal;

    textureCoords = v_texture;

    vec3 _normal = normalize(v_normal);
    vec3 _tangent = normalize(v_tangent);
    _tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
    vec3 _bitangent = cross(_normal, _tangent);

    vec3 T = normalize(vec3(normalMatrix * _tangent));
    vec3 B = normalize(vec3(normalMatrix * _bitangent));
    vec3 N = normalize(vec3(normalMatrix * _normal));
    tbn = mat3(T, B, N);
}