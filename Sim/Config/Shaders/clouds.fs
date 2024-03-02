#version 330 core

//// Standard lighting uniforms
// Material properties
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularMult;

// Directional light properties
uniform vec3 dLightDirection;
uniform vec3 dLightAmbient;
uniform vec3 dLightDiffuse;

// Point light properties. Current max of 1 light
uniform vec3 pLightPosition;
uniform vec3 pLightAmbient;
uniform vec3 pLightDiffuse;
//// End of standard lighting uniforms

in vec3 fs_position;
in vec3 fs_normal;

out vec4 color;

void main(void) {
    //// Standard lighting block
    vec3 normal = normalize(fs_normal);

    vec3 pointLightDistance = pLightPosition - fs_position;
    float pointLightStrength = max(1.0, 100.0 / (length(pointLightDistance) + 0.01));

    // Ambient (diffuse + point light reduction)
    vec3 overallAmbient = ambient * (dLightAmbient + pLightAmbient * pointLightStrength);

    vec3 directionToPointLight = normalize(pointLightDistance);
    
    float dLightDotProd = dot(dLightDirection, normal);
    float pLightDotProd = dot(directionToPointLight, normal);
    
    // Diffuse
    vec3 overallDiffuse = diffuse * (
        dLightDiffuse * abs(max(dLightDotProd, 0)) + 
        pLightDiffuse * pointLightStrength * abs(max(pLightDotProd, 0)));

    // Specular
    vec3 dReflection = reflect(-dLightDirection, normal);
    vec3 pReflection = reflect(-directionToPointLight, normal);
  
    float dSpecular = dot(dReflection, -normalize(fs_position));
    float pSpecular = dot(pReflection, -normalize(fs_position));

    vec3 overallSpecular = specular * (pow(abs(max(dSpecular, 0)), specularMult) +
        pointLightStrength * pow(abs(max(pSpecular, 0)), specularMult));

    vec3 defaultLighting = overallAmbient + overallDiffuse + overallSpecular;
    //// End of standard lighting block

    color = vec4(defaultLighting, 1.0);
}