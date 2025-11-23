#version 330

// ========================================
// INPUTS
// ========================================
in vec3 worldFragmentPosition;
in vec3 worldNormal;
in vec2 textureCoords;

out vec4 outColor;

// ========================================
// MATERIAL STRUCTURE
// ========================================
struct Material {
    vec3 ambient;    // ra koeficient - ovplyvní ambient textúry
    vec3 diffuse;    // rd koeficient - ovplyvní diffuse textúry
    vec3 specular;   // rs koeficient - ovplyvní lesk (BEZ textúry!)
    float shininess; // h koeficient - ostrosť lesku
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
uniform sampler2D tex;  // Textúra

uniform vec3 viewPosition;
uniform vec3 cameraDirection;

// Material uniforms
uniform Material material;

// Fallback pre backward compatibility
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
// HELPER FUNCTIONS
// ========================================

float calculateAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

float calculateDiffuse(vec3 lightVector, vec3 normal)
{
    return max(dot(lightVector, normal), 0.0);
}

float calculateSpecular(vec3 lightVector, vec3 cameraVector, vec3 normal, float shininess)
{
    if (dot(lightVector, normal) >= 0.0)
    {
        vec3 reflectDir = reflect(-lightVector, normal);
        return pow(max(dot(reflectDir, cameraVector), 0.0), shininess);
    }
    return 0.0;
}

// ========================================
// LIGHT CALCULATIONS WITH MATERIAL + TEXTURE
// ========================================

vec3 calcPointLight(PointLight light, vec3 cameraVector, vec3 normal, vec3 textureColor)
{
    vec3 lightVector = normalize(light.position - worldFragmentPosition);

    // Ambient - textúra * material.ambient
    vec3 ambientComponent = light.color * material.ambient * textureColor;

    // Diffuse - textúra * material.diffuse
    float diff = calculateDiffuse(lightVector, normal);
    vec3 diffuseComponent = light.color * (diff * material.diffuse * textureColor);

    // Specular - BEZ textúry! Iba material.specular
    float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
    vec3 specularComponent = light.color * (spec * material.specular);

    // Attenuation
    float distance = length(light.position - worldFragmentPosition);
    float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

    return attenuation * (ambientComponent + diffuseComponent + specularComponent);
}

vec3 calcSpotlightCamera(SpotlightCamera light, vec3 cameraVector, vec3 normal, vec3 textureColor)
{
    vec3 lightPosition = viewPosition;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(-cameraDirection));

    if (theta > light.angle)
    {
        // Ambient
        vec3 ambientComponent = light.color * material.ambient * textureColor;

        // Diffuse
        float diff = calculateDiffuse(lightVector, normal);
        vec3 diffuseComponent = light.color * (diff * material.diffuse * textureColor);

        // Specular
        float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
        vec3 specularComponent = light.color * (spec * material.specular);

        // Attenuation
        float distance = length(viewPosition - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

        return attenuation * (ambientComponent + diffuseComponent + specularComponent);
    }
    return vec3(0.0);
}

vec3 calcSpotlight(Spotlight light, vec3 cameraVector, vec3 normal, vec3 textureColor)
{
    vec3 lightPosition = light.position;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(light.direction));

    if (theta > light.angle)
    {
        // Ambient
        vec3 ambientComponent = light.color * material.ambient * textureColor;

        // Diffuse
        float diff = calculateDiffuse(lightVector, normal);
        vec3 diffuseComponent = light.color * (diff * material.diffuse * textureColor);

        // Specular
        float spec = calculateSpecular(lightVector, cameraVector, normal, material.shininess);
        vec3 specularComponent = light.color * (spec * material.specular);

        // Attenuation
        float distance = length(light.position - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

        return attenuation * (ambientComponent + diffuseComponent + specularComponent);
    }
    return vec3(0.0);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 cameraVector, vec3 normal, vec3 textureColor)
{
    vec3 lightVector = normalize(light.direction);

    // Ambient
    vec3 ambientComponent = light.color * material.ambient * textureColor;

    // Diffuse
    float diff = calculateDiffuse(lightVector, normal);
    vec3 diffuseComponent = light.color * (diff * material.diffuse * textureColor);

    // Specular
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

    // Získaj farbu z textúry
    vec3 textureColor = texture(tex, textureCoords).rgb;

    vec3 result = vec3(0.0);

    // Point lights
    for(int i = 0; i < numberOfPointLights; i++)
    {
        result += calcPointLight(pointlights[i], cameraVector, normal, textureColor);
    }

    // Spotlights
    for(int i = 0; i < numberOfSpotlights; i++)
    {
        result += calcSpotlight(spotlights[i], cameraVector, normal, textureColor);
    }

    // Spotlight camera
    if (spotlightCamera_bool)
    {
        result += calcSpotlightCamera(spotlightCamera, cameraVector, normal, textureColor);
    }

    // Directional light
    if (directional_bool)
    {
        result += calcDirectionalLight(directional, cameraVector, normal, textureColor);
    }

    // Fallback ambient if no lights
    if (numberOfPointLights == 0 && numberOfSpotlights == 0 && !spotlightCamera_bool && !directional_bool)
    {
        result = material.ambient * textureColor * ambient;
    }

    outColor = vec4(result, 1.0);
}
