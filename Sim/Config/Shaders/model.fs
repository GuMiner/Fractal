#version 330 core

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

in vec3 fs_position;
in vec3 fs_normal;

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

  vec3 overallSpecular = specular * (pow(abs(max(dSpecular, 0)), specularMult) +
      pow(abs(max(pSpecular, 0)), specularMult));

    color = vec4(overallAmbient + overallDiffuse + overallSpecular, 1.0f); //  + overallSpecular, 1.0f);
}
