#version 330

in vec3 worldFragmentPosition;

uniform samplerCube tex;  // Cubemap textúra (6 strán kocky)

out vec4 outColor;

void main () { 
    // Sampluj cubemap textúru pomocou 3D súradníc
    outColor = texture(tex, worldFragmentPosition);
}
