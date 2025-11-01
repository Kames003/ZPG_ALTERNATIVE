#version 330

// INPUT z vertex shadera
in vec3 worldFragmentPosition;
in vec3 worldNormal;

// OUTPUT
out vec4 out_Color;

// ========================================
// LIGHT STRUCTURES
// ========================================
#define MAX_LIGHTS 16  // ✅ Pre mesiac + 12 svetlušiek

struct PointLight {
    vec3 position;      // ✅ vec3 (nie vec4!)
    vec3 color;         // ✅ Farba svetla
    float constant;     // ✅ Útlum
    float linear;
    float quadratic;
};

// ========================================
// UNIFORMS
// ========================================
uniform vec3 viewPosition;      // Pozícia kamery
uniform vec3 objectColor;       // Farba objektu (strom, zem, atď.)

uniform int numberOfPointLights;
uniform PointLight pointlights[MAX_LIGHTS];

uniform float ambient;          // Globálne ambient osvětlenie

// ========================================
// HELPER FUNCTIONS
// ========================================

float calculateAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

// ========================================
// MAIN - PHONG SHADING
// ========================================
void main()
{
    // Normalizované vektory
    vec3 normal = normalize(worldNormal);
    vec3 viewDir = normalize(viewPosition - worldFragmentPosition);

    // ✅ Globálne ambient pre minimálne osvetlenie (noč)
    vec3 globalAmbient = objectColor * ambient;

    // Akumulácia diffuse a specular
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    // ✅ CYKLUS CEZ VŠETKY SVETLÁ
    for (int i = 0; i < numberOfPointLights; i++)
    {
        PointLight light = pointlights[i];

        // Vektor k svetlu
        vec3 lightDir = normalize(light.position - worldFragmentPosition);
        float distance = length(light.position - worldFragmentPosition);

        // ✅ ÚTLUM (attenuation)
        float attenuation = calculateAttenuation(distance,
                                                 light.constant, light.linear, light.quadratic);

        // DIFFUSE - kontrola či svetlo svítí na túto stranu
        float dotNL = dot(normal, lightDir);

        if (dotNL > 0.0)
        {
            // ✅ DIFFUSE component
            vec3 diffuse = dotNL * objectColor * light.color;

            // ✅ SPECULAR component (Phong reflection)
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

            // Pre svetlušky (i > 0) používame slabší specular
            float specularStrength = (i == 0) ? 0.3 : 0.1;
            vec3 specular = spec * light.color * specularStrength;

            // Aplikuj útlum
            totalDiffuse += attenuation * diffuse;
            totalSpecular += attenuation * specular;
        }
        // Ak dotNL <= 0 → odvrácená strana, žiadne osvetlenie
    }

    // ✅ Finálna farba
    vec3 result = globalAmbient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}