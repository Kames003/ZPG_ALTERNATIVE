#version 330

layout(location=0) in vec3 v_position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 worldFragmentPosition;

void main () {
    vec4 position = modelMatrix * vec4(v_position, 1.0);
    worldFragmentPosition = position.xyz / position.w;
    
    // Skybox trick: odstránime transláciu z view matrix (len rotácia zostane)
    // mat4(mat3(viewMatrix)) odstráni posledný stĺpec a riadok (transláciu)
    gl_Position = (projectionMatrix * mat4(mat3(viewMatrix)) * modelMatrix) * vec4(v_position, 1.0);
}
