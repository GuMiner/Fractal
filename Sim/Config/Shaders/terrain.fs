#version 330 core

// Material properties
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specularColor;
uniform float specular;

// Directional light properties
uniform vec3 dLightDirection;
uniform vec3 dLightAmbient;
uniform vec3 dLightDiffuse;

// Point light properties. Current max of 1 light
uniform vec3 pLightPosition;
uniform vec3 pLightAmbient;
uniform vec3 pLightDiffuse;

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
    vec3 normal = normalize(fs_normal);

    vec3 overallAmbient = ambient * (dLightAmbient + pLightAmbient);

    vec3 directionToPointLight = normalize(pLightPosition - fs_position);
    //
    float dLightDotProd = dot(dLightDirection, normal);
    float pLightDotProd = dot(directionToPointLight, normal);
    //
    //// Max of 0 to not light interior surfaces
    vec3 overallDiffuse = diffuse * (
        dLightDiffuse * abs(max(dLightDotProd, 0)) + 
        pLightDiffuse * abs(max(pLightDotProd, 0)));
    //
  vec3 dReflection = reflect(-dLightDirection, normal);
  vec3 pReflection = reflect(-directionToPointLight, normal);
  
  float dSpecular = dot(dReflection, -normalize(fs_position));
  float pSpecular = dot(pReflection, -normalize(fs_position));

  vec3 overallSpecular = specularColor * (pow(abs(max(dSpecular, 0)), specular) +
      pow(abs(max(pSpecular, 0)), specular));

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

  color = colorMultiplier * vec4(overallAmbient + overallDiffuse + overallSpecular, 1.0f); //  + overallSpecular, 1.0f);
}
