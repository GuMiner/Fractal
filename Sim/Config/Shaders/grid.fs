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

in float edgeFactor;
in vec3 fs_position;
in vec3 fs_normal;

out vec4 color;

void main(void) {
    vec3 normal = normalize(fs_normal);
    vec3 overallAmbient = ambient * (dLightAmbient) * 0.25f;

    //
    float dLightDotProd = dot(dLightDirection, normal);
    //
    //// Max of 0 to not light interior surfaces
    vec3 overallDiffuse = diffuse * (
        dLightDiffuse * abs(max(dLightDotProd, 0)));
    //
  vec3 dReflection = reflect(-dLightDirection, normal);
  
    float dSpecular = dot(dReflection, -normalize(fs_position));

  vec3 overallSpecular = specularColor * pow(abs(max(dSpecular, 0)), specular);
    color = mix(vec4(overallAmbient + overallDiffuse + overallSpecular, 1.0), vec4(1.0), edgeFactor * 2);
}