#version 330

// in z vertexshadera
in vec3 worldFragmentPosition;
in vec3 worldNormal;
in vec2 textureCoords;
in mat3 tbn;

out vec4 outColor; // out farba pixelu

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
uniform sampler2D tex;
uniform sampler2D normalMap;

uniform vec3 viewPosition;
uniform vec3 cameraDirection;
uniform float ambient;

uniform int numberOfPointLights;
uniform PointLight pointlights[MAX_POINT_LIGHTS];

uniform int numberOfSpotlights;
uniform Spotlight spotlights[3];

uniform bool spotlightCamera_bool = false;
uniform SpotlightCamera spotlightCamera;

uniform bool directional_bool = false;
uniform DirectionalLight directional;

uniform float normalMapIntensity = 1.0;

// ========================================
// HELPER FUNCTIONS
// ========================================

float calculateAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / ( constant + linear * distance + quadratic * distance * distance );
}

float calculateDiffuse(vec3 lightVector, vec3 normal)
{
    return max(dot(lightVector, normal), 0.0);
}

float calculateSpecular(vec3 lightVector, vec3 normal, vec3 cameraVector)
{
    if ( dot(lightVector, normal) >= 0.0 )
    {
        return pow(max(dot(reflect(-lightVector, normal), cameraVector), 0.0), 16.0);
    } else {
        return 0.0;
    }
}

// ========================================
// LIGHT CALCULATIONS
// ========================================

vec4 calcPointLight(PointLight l, vec3 normal, vec3 cameraVector)
{
    vec3 lightVector = normalize ( l.position - worldFragmentPosition );

    float diffuse = calculateDiffuse(lightVector, normal);
    float specular = calculateSpecular(lightVector, normal, cameraVector);

    float distance = length(l.position - worldFragmentPosition);
    float attenuation = calculateAttenuation(distance, l.constant, l.linear, l.quadratic);

    return attenuation * ( diffuse * vec4(l.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0) );
}

vec4 calcSpotlightCamera(SpotlightCamera s, vec3 normal, vec3 cameraVector)
{
    vec3 lightPosition = viewPosition;
    vec3 lightVector = normalize ( lightPosition - worldFragmentPosition );

    float theta = dot(lightVector, normalize(-cameraDirection));

    if (theta > s.angle)
    {
        float diffuse = calculateDiffuse(lightVector, normal);
        float specular = calculateSpecular(lightVector, normal, cameraVector);
        float distance = length(viewPosition - worldFragmentPosition);
        float attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);

        return attenuation * ( diffuse * vec4(s.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0) );
    }
    return vec4(0.0);
}

vec4 calcSpotlight(Spotlight s, vec3 normal, vec3 cameraVector)
{
    vec3 lightPosition = s.position;
    vec3 lightVector = normalize ( lightPosition - worldFragmentPosition );

    float theta = dot(lightVector, normalize(s.direction));

    if (theta > s.angle)
    {
        float diffuse = calculateDiffuse(lightVector, normal);
        float specular = calculateSpecular(lightVector, normal, cameraVector);
        float distance = length( s.position - worldFragmentPosition );
        float attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);

        return attenuation * ( diffuse * vec4(s.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0) );
    }
    return vec4(0.0);
}

vec4 calcDirectionalLight(DirectionalLight d, vec3 normal, vec3 cameraVector)
{
    vec3 lightVector = normalize ( d.direction );
    float diffuse = calculateDiffuse(lightVector, normal);
    float specular = calculateSpecular(lightVector, normal, cameraVector);

    return diffuse * vec4(d.color, 1.0) + specular * vec4(1.0, 1.0, 1.0, 1.0);
}

// ========================================
// MAIN
// ========================================

void main(){
    vec3 cameraVector = normalize ( viewPosition - worldFragmentPosition );

    // Normal mapping
    vec3 encodedNormal = texture(normalMap, textureCoords).rgb;
    encodedNormal = 2.0 * encodedNormal - 1.0; // RGB to vector
    // intensity
    encodedNormal = normalize(encodedNormal * vec3(1.0, 1.0, normalMapIntensity));
    vec3 normal = normalize(tbn * encodedNormal);

    vec4 point = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 spot = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 spotlightCam = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 directionalLight = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i < numberOfPointLights; i++)
        point += calcPointLight(pointlights[i], normal, cameraVector);

    for(int i = 0; i < numberOfSpotlights; i++)
        spot += calcSpotlight(spotlights[i], normal, cameraVector);

    if (spotlightCamera_bool)
    {
        spotlightCam = calcSpotlightCamera(spotlightCamera, normal, cameraVector);
    }

    if (directional_bool)
    {
        directionalLight = calcDirectionalLight(directional, normal, cameraVector);
    }

    outColor = texture(tex, textureCoords) * ( directionalLight + point + spot + spotlightCam + ambient );
}