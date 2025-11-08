#version 330

// ========================================
// INPUTS
// ========================================
in vec3 worldFragmentPosition;
in vec3 worldNormal;

out vec4 outColor;

// ========================================
// MATERIAL STRUCTURE
// ========================================
struct Material {
    vec3 ambient;    // ra koeficient
    vec3 diffuse;    // rd koeficient  
    vec3 specular;   // rs koeficient
    float shininess; // h koeficient
};

// ========================================
// LIGHT STRUCTURES
// ========================================
#define MAX_POINT_LIGHTS 24

struct PointLight {
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotlightCamera {
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float angle;
};

struct Spotlight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float angle;
};

struct DirectionalLight {
    vec3 color;
    vec3 direction;
};

// ========================================
// UNIFORMS
// ========================================
uniform vec3 viewPosition;
uniform vec3 cameraDirection;

// Material uniforms
uniform Material material;

// Fallback pre backward compatibility
uniform vec3 objectColor;
uniform float ambient;

// Lights
uniform int numberOfPointLights;
uniform PointLight pointlights[MAX_POINT_LIGHTS];

uniform int numberOfSpotlights;
uniform Spotlight spotlights[3];

uniform bool spotlightCamera_bool = false;
uniform SpotlightCamera spotlightCamera;

uniform bool directional_bool = false;
uniform DirectionalLight directional;

// ========================================
// HELPER FUNCTIONS (BLINN-PHONG)
// ========================================

float calculateAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

float calculateDiffuse(vec3 lightVector, vec3 normal)
{
    return max(dot(lightVector, normal), 0.0);
}

// BLINN-PHONG: Používa half-vector namiesto reflect
float calculateSpecular(vec3 lightVector, vec3 cameraVector, vec3 normal, float shininess)
{
    if (dot(lightVector, normal) >= 0.0)
    {
        vec3 halfVector = normalize(lightVector + cameraVector);
        return pow(max(dot(normal, halfVector), 0.0), shininess);
    }
    return 0.0;
}

// ========================================
// LIGHT CALCULATIONS WITH MATERIAL
// ========================================

vec3 calcPointLight(PointLight light, vec3 cameraVector, vec3 normal)
{
    vec3 lightVector = normalize(light.position - worldFragmentPosition);

    // Ambient
    vec3 ambientComponent = light.color * material.ambient;

    // Diffuse
    float diff = calculateDiffuse(lightVector, normal);
    vec3 diffuseComponent = light.color * (diff * material.diffuse);

    // Specular (Blinn-Phong)
    float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
    vec3 specularComponent = light.color * (spec * material.specular);

    // Attenuation
    float distance = length(light.position - worldFragmentPosition);
    float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

    return attenuation * (ambientComponent + diffuseComponent + specularComponent);
}

vec3 calcSpotlightCamera(SpotlightCamera light, vec3 cameraVector, vec3 normal)
{
    vec3 lightPosition = viewPosition;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(-cameraDirection));

    if (theta > light.angle)
    {
        // Ambient
        vec3 ambientComponent = light.color * material.ambient;

        // Diffuse
        float diff = calculateDiffuse(lightVector, normal);
        vec3 diffuseComponent = light.color * (diff * material.diffuse);

        // Specular (Blinn-Phong)
        float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
        vec3 specularComponent = light.color * (spec * material.specular);

        // Attenuation
        float distance = length(viewPosition - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

        return attenuation * (ambientComponent + diffuseComponent + specularComponent);
    }
    return vec3(0.0);
}

vec3 calcSpotlight(Spotlight light, vec3 cameraVector, vec3 normal)
{
    vec3 lightPosition = light.position;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(light.direction));

    if (theta > light.angle)
    {
        // Ambient
        vec3 ambientComponent = light.color * material.ambient;

        // Diffuse
        float diff = calculateDiffuse(lightVector, normal);
        vec3 diffuseComponent = light.color * (diff * material.diffuse);

        // Specular (Blinn-Phong)
        float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
        vec3 specularComponent = light.color * (spec * material.specular);

        // Attenuation
        float distance = length(light.position - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

        return attenuation * (ambientComponent + diffuseComponent + specularComponent);
    }
    return vec3(0.0);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 cameraVector, vec3 normal)
{
    vec3 lightVector = normalize(light.direction);

    // Ambient
    vec3 ambientComponent = light.color * material.ambient;

    // Diffuse
    float diff = calculateDiffuse(lightVector, normal);
    vec3 diffuseComponent = light.color * (diff * material.diffuse);

    // Specular (Blinn-Phong)
    float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
    vec3 specularComponent = light.color * (spec * material.specular);

    return ambientComponent + diffuseComponent + specularComponent;
}

// ========================================
// MAIN
// ========================================

void main()
{
    vec3 normal = normalize(worldNormal);
    vec3 cameraVector = normalize(viewPosition - worldFragmentPosition);

    vec3 result = vec3(0.0);

    // Point lights
    for(int i = 0; i < numberOfPointLights; i++)
    {
        result += calcPointLight(pointlights[i], cameraVector, normal);
    }

    // Spotlights
    for(int i = 0; i < numberOfSpotlights; i++)
    {
        result += calcSpotlight(spotlights[i], cameraVector, normal);
    }

    // Spotlight camera
    if (spotlightCamera_bool)
    {
        result += calcSpotlightCamera(spotlightCamera, cameraVector, normal);
    }

    // Directional light
    if (directional_bool)
    {
        result += calcDirectionalLight(directional, cameraVector, normal);
    }

    // Fallback ambient if no lights
    if (numberOfPointLights == 0 && numberOfSpotlights == 0 && !spotlightCamera_bool && !directional_bool)
    {
        result = material.ambient * ambient;
    }

    outColor = vec4(result, 1.0);
}
