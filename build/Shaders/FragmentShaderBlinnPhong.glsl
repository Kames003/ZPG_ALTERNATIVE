#version 330

in vec3 worldFragmentPosition;  // ✅ vec3 ako ostatné!
in vec3 worldNormal;

out vec4 outColor;

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

uniform vec3 viewPosition;       // ✅ PRIDANÉ
uniform vec3 cameraDirection;    // ✅ PRIDANÉ
uniform vec3 objectColor;        // ✅ PRIDANÉ

uniform float ambient;           // ✅ PRIDANÉ

uniform int numberOfPointLights; // ✅ PRIDANÉ
uniform PointLight pointlights[3];

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

float calculateDiffuse(vec3 lightVector)
{
    return max(dot(lightVector, worldNormal), 0.0);
}

// ✅ KĽÚČOVÝ ROZDIEL: Half-vector namiesto reflect!
float calculateSpecular(vec3 lightVector, vec3 cameraVector)
{
    if (dot(lightVector, worldNormal) >= 0)
    {
        // ✅ BLINN-PHONG: Používa half-vector
        vec3 halfVector = normalize(lightVector + cameraVector);
        return pow(max(dot(worldNormal, halfVector), 0.0), 32.0);  // ✅ Vyšší exponent pre ostrejší highlight
    } else {
        return 0.0;
    }
}

// ========================================
// LIGHT CALCULATIONS
// ========================================

vec4 calcPointLight(PointLight l, vec3 cameraVector)
{
    vec3 lightVector = normalize(l.position - worldFragmentPosition);

    float diffuse = calculateDiffuse(lightVector);
    float specular = calculateSpecular(lightVector, cameraVector);

    float distance = length(l.position - worldFragmentPosition);
    float attenuation = calculateAttenuation(distance, l.constant, l.linear, l.quadratic);

    return attenuation * (diffuse * vec4(l.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0));
}

vec4 calcSpotlightCamera(SpotlightCamera s, vec3 cameraVector)
{
    vec3 lightPosition = viewPosition;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(-cameraDirection));

    if (theta > s.angle)
    {
        float diffuse = calculateDiffuse(lightVector);
        float specular = calculateSpecular(lightVector, cameraVector);
        float distance = length(viewPosition - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);

        return attenuation * (diffuse * vec4(s.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0));
    }
    return vec4(0.0);
}

vec4 calcSpotlight(Spotlight s, vec3 cameraVector)
{
    vec3 lightPosition = s.position;
    vec3 lightVector = normalize(lightPosition - worldFragmentPosition);

    float theta = dot(lightVector, normalize(s.direction));

    if (theta > s.angle)
    {
        float diffuse = calculateDiffuse(lightVector);
        float specular = calculateSpecular(lightVector, cameraVector);
        float distance = length(s.position - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);

        return attenuation * (diffuse * vec4(s.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0));
    }
    return vec4(0.0);
}

vec4 calcDirectionalLight(DirectionalLight d, vec3 cameraVector)
{
    vec3 lightVector = normalize(d.direction);
    float diffuse = calculateDiffuse(lightVector);
    float specular = calculateSpecular(lightVector, cameraVector);

    return diffuse * vec4(d.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0);
}

// ========================================
// MAIN
// ========================================

void main()
{
    vec3 cameraVector = normalize(viewPosition - worldFragmentPosition);

    vec4 point = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 spot = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 spotlightCam = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 directionalLight = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i < numberOfPointLights; i++)
    point += calcPointLight(pointlights[i], cameraVector);

    for(int i = 0; i < numberOfSpotlights; i++)
    spot += calcSpotlight(spotlights[i], cameraVector);

    if (spotlightCamera_bool)
    {
        spotlightCam = calcSpotlightCamera(spotlightCamera, cameraVector);
    }

    if (directional_bool)
    {
        directionalLight = calcDirectionalLight(directional, cameraVector);
    }

    outColor = vec4(objectColor, 1.0) * (directionalLight + point + spot + spotlightCam + ambient);
}