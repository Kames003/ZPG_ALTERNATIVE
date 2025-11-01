#version 330

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 outColor;

uniform vec3 cameraPosition;

void main(){
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);
    vec3 lightVector = normalize ( vec3(lightPosition.x - worldPosition.x, lightPosition.y - worldPosition.y, lightPosition.z - worldPosition.z) );
    vec3 cameraVector = normalize ( vec3(cameraPosition.x - worldPosition.x, cameraPosition.y - worldPosition.y, cameraPosition.z - worldPosition.z) );
    vec3 halfVector = normalize( lightVector + cameraVector );

    float ambient = 0.2;
    float diffuse = max(dot(lightVector, worldNormal), 0.0);
    float specular = pow(max(dot(worldNormal, halfVector), 0.0), 16);

    outColor = vec4(0.91, 0.47, 0.65, 1.0) * ( ambient + diffuse + specular );
}