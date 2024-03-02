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

uniform vec3 WaterColor;
uniform float WaterGrassCutoff;
uniform vec3 GrassColor;
uniform float GrassForestCutoff;
uniform vec3 ForestColor;
uniform float ForestTundraCutoff;
uniform vec3 TundraColor;
uniform float TundraRockCutoff;
uniform vec3 RockColor;
uniform float RockSnowCutoff;
uniform vec3 SnowColor;

in vec3 fs_position;
in vec3 fs_normal;
in float fs_heightPercentage;

out vec4 color;

void main() {
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

  vec4 colorMultiplier = vec4(WaterColor, 1.0f);
  if (fs_heightPercentage > RockSnowCutoff) {
    colorMultiplier = vec4(SnowColor, 1.0f);
  } else if (fs_heightPercentage > TundraRockCutoff) {
    colorMultiplier = vec4(RockColor, 1.0f);
  } else if (fs_heightPercentage > ForestTundraCutoff) {
    colorMultiplier = vec4(TundraColor, 1.0f);
  } else if (fs_heightPercentage > GrassForestCutoff) {
    colorMultiplier = vec4(ForestColor, 1.0f);
  } else if (fs_heightPercentage > WaterGrassCutoff) {
    colorMultiplier = vec4(GrassColor, 1.0f);
  } // else, Water color

  colorMultiplier = vec4(1.0f); // TODO remove to add back in colors
  color = colorMultiplier * vec4(defaultLighting, 1.0f);
}
