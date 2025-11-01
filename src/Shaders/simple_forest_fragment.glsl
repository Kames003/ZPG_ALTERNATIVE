#version 330

// INPUT z vertex shadera
in vec4 worldPosition;
in vec3 worldNormal;

// OUTPUT
out vec4 out_Color;

// ========================================
// LIGHT STRUCTURES
// ========================================
#define MAX_LIGHTS 8

struct Light {
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

// ========================================
// UNIFORMS
// ========================================
uniform vec3 viewPosition;      // ✅ Pozícia kamery
uniform vec3 objectColor;       // ✅ Farba objektu
uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;

// ========================================
// MAIN - PHONG SHADING
// ========================================
void main()
{
    // Globálne ambient pre minimálne osvetlenie
    vec3 globalAmbient = objectColor * 0.15;

    // Ambient z prvého svetla
    vec3 ambient = vec3(0.0);
    if (numberOfLights > 0) {
        ambient = lights[0].ambient * objectColor * 0.1;
    }

    // Akumulácia diffuse a specular
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    // Smer pohľadu z fragmentu ku kamere
    vec3 viewDir = normalize(viewPosition - worldPosition.xyz);

    // Cyklus cez všetky svetlá
    for (int i = 0; i < numberOfLights; i++) {
        Light light = lights[i];

        // Vektor k svetlu
        vec3 lightDir = normalize(light.position.xyz - worldPosition.xyz);

        // Normalizovaná normála
        vec3 norm = normalize(worldNormal);

        // DIFFUSE - kontrola či svetlo svieti na túto stranu
        float dotNL = dot(norm, lightDir);

        if (dotNL > 0.0) {
            // DIFFUSE
            vec3 diffuse = dotNL * objectColor * light.diffuse * light.intensity;

            // SPECULAR - Phong reflection model
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * light.specular * light.intensity * 0.5;

            // ATTENUATION (útlm so vzdialenosťou)
            float distance = length(light.position.xyz - worldPosition.xyz);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

            totalDiffuse += attenuation * diffuse;
            totalSpecular += attenuation * specular;
        }
    }

    // Finálna farba
    vec3 result = globalAmbient + ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}