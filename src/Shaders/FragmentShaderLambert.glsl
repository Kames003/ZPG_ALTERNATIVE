#version 330

in vec3 worldFragmentPosition;
in vec3 worldNormal;

out vec4 outColor;

uniform sampler2D texture_unit;

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

uniform vec3 cameraPosition;
uniform vec3 cameraDirection;

uniform float ambient;

uniform int numberOfPointLights;
uniform PointLight pointlights[3];

uniform int numberOfSpotlights;
uniform Spotlight spotlights[3];

uniform bool spotlightCamera_bool = false;
uniform SpotlightCamera spotlightCamera;

uniform bool directional_bool = false;
uniform DirectionalLight directional;

float calculateAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / ( constant + linear * distance + quadratic * distance * distance );
}

float calculateDiffuse(vec3 lightVector)
{
    return max(dot(lightVector, worldNormal), 0.0);
}

vec4 calcPointLight(PointLight l)
{
    vec3 lightVector = normalize ( l.position - worldFragmentPosition );

    float diffuse = calculateDiffuse(lightVector);

    float distance = length(l.position - worldFragmentPosition);
    float attenuation = calculateAttenuation(distance, l.constant, l.linear, l.quadratic);

    return attenuation * ( diffuse * vec4(l.color, 1.0) );
}

vec4 calcSpotlightCamera(SpotlightCamera s)
{
    vec3 lightPosition = cameraPosition;
    vec3 lightVector = normalize ( lightPosition - worldFragmentPosition );

    float theta = dot(lightVector, normalize(-cameraDirection));

    float diffuse = 0;
    float distance = 0;
    float attenuation = 0;

    if (theta > s.angle)
    {
        diffuse = calculateDiffuse(lightVector);
        distance = length(cameraPosition - worldFragmentPosition);
        attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);
    }

    return attenuation * ( diffuse * vec4(s.color, 1.0) );
}

vec4 calcSpotlight(Spotlight s)
{
    vec3 lightPosition = s.position;
    vec3 lightVector = normalize ( s.position - worldFragmentPosition );

    float theta = dot(lightVector, normalize(s.direction));

    float diffuse = 0;
    float distance = 0;
    float attenuation = 0;

    if (theta > s.angle)
    {
        diffuse = calculateDiffuse(lightVector);
        distance = length(s.position - worldFragmentPosition);
        attenuation = calculateAttenuation(distance, s.constant, s.linear, s.quadratic);
    }

    return attenuation * ( diffuse * vec4(s.color, 1.0) );
}

vec4 calcDirectionalLight(DirectionalLight d)
{
    vec3 lightVector = normalize ( d.direction );
    float diffuse = calculateDiffuse(lightVector);

    return diffuse * vec4(d.color, 1.0);
}

void main() {
    vec4 point = vec4(0.0, 0.0f, 0.0f, 1.0f);
    vec4 spot = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 spotlightCam = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 directionalLight = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    for(int i = 0; i < numberOfPointLights; i++)
    point += calcPointLight(pointlights[i]);

    for(int i = 0; i < numberOfSpotlights; i++)
    spot += calcSpotlight(spotlights[i]);

    if (spotlightCamera_bool)
    {
        spotlightCam = calcSpotlightCamera(spotlightCamera);
    }

    if (directional_bool)
    {
        directionalLight = calcDirectionalLight(directional);
    }

    outColor = vec4(0.385, 0.647, 0.812, 1.0) * ( directionalLight + point + spot + spotlightCam + ambient );
}