#version 330

// TEST SHADER - Overenie homogennej zlozky w (s texturou)
// Zadanie: uniform float w = 500 a overit ci vsetko funguje spravne

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
    // === TEST HOMOGENNEJ SURADNICE ===
    // Zadanie: Zadejte si v shaderu promennou w = 500
    float w = 500.0;  // LOKALNA premenna (ako u kolegu)

    // World space pozicia pre osvetlenie
    vec4 worldPos = modelMatrix * vec4(v_position, 1.0);
    worldFragmentPosition = worldPos.xyz / worldPos.w;
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * v_normal);

    // Texturove suradnice
    textureCoords = v_texture;

    // === TEST W = 500 ===
    // Skalovanie VNUTRI vec4: vec4(pos * w, w) = (x*500, y*500, z*500, 500)
    // Po transformaciach sa vsetko skrati pri perspektivnom deleni
    // Vysledok: objekt sa NEZMENI
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position * w, w);
}
