#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include "Experimental.h"

using namespace glm;
using namespace std;

const vec3 cameraPos = vec3(0, 0, 0); //vec3(100000.0, -40000.0, 0);
const vec3 sunPosition = vec3(0, -700000, 0);

// "Red sunset" values
const float depolarizationFactor = 0.02;
const float luminance = 1.0;
const float mieCoefficient = 0.005;
const float mieDirectionalG = 0.82;
const vec3 mieKCoefficient = vec3(0.686, 0.678, 0.666);
const float mieV = 3.936;
const float mieZenithLength = 34000;
const float numMolecules = 2.542e25;
const vec3 primaries = vec3(6.8e-7, 5.5e-7, 4.5e-7);
const float rayleigh = 2.28;
const float rayleighZenithLength = 8400;
const float refractiveIndex = 1.00029;
const float sunAngularDiameterDegrees = 0.00933;
const float sunIntensityFactor = 1000;
const float sunIntensityFalloffSteepness = 1.5;
const float tonemapWeighting = 9.50;
const float turbidity = 4.7;

const float PI = 3.141592653589793238462643383279502884197169;
const vec3 UP = vec3(0.0, 1.0, 0.0);

float max(float a, float b) {
    if (a > b) {
        return a;
    }
    
    return b;
}

glm::vec3 pow(glm::vec3 val, glm::vec3 powVal) {
    return glm::vec3(pow(val.x, powVal.x), pow(val.y, powVal.y), pow(val.z, powVal.z));
}

vec3 totalRayleigh(vec3 lambda)
{
    return (8.0f * pow(PI, 3.0f) * pow(pow(refractiveIndex, 2.0f) - 1.0f, 2.0f) * (6.0f + 3.0f * depolarizationFactor)) / 
        (3.0f * numMolecules * pow(lambda, vec3(4.0f)) * (6.0f - 7.0f * depolarizationFactor));
}

vec3 totalMie(vec3 lambda, vec3 K, float T)
{
    float c = 0.2 * T * 10e-18;
    return 0.434f * c * PI * pow((2.0f * PI) / lambda, vec3(mieV - 2.0f)) * K;
}

float rayleighPhase(float cosTheta)
{
    return (3.0 / (16.0 * PI)) * (1.0 + pow(cosTheta, 2.0));
}

float henyeyGreensteinPhase(float cosTheta, float g)
{
    return (1.0 / (4.0 * PI)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0 * g * cosTheta + pow(g, 2.0), 1.5));
}

float sunIntensity(float zenithAngleCos)
{
    float cutoffAngle = PI / 1.95; // Earth shadow hack
    return sunIntensityFactor * max(0.0, 1.0 - exp(-((cutoffAngle - acos(zenithAngleCos)) / sunIntensityFalloffSteepness)));
}

// Whitescale tonemapping calculation, see http://filmicgames.com/archives/75
// Also see http://blenderartists.org/forum/showthread.php?321110-Shaders-and-Skybox-madness
const float A = 0.15; // Shoulder strength
const float B = 0.50; // Linear strength
const float C = 0.10; // Linear angle
const float D = 0.20; // Toe strength
const float E = 0.02; // Toe numerator
const float F = 0.30; // Toe denominator
vec3 Uncharted2Tonemap(vec3 W)
{
    return ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
}

void Experimental::Test() {
    vec3 vWorldPosition = vec3(0, 0, 1);//fs_pos;
    // TODO assumes a sphere of size 450,000. Needs to be adjusted for flat projection

    // Rayleigh coefficient
    float sunfade = 0.4; // - clamp(1.0 - exp((sunPosition.y / 450000.0)), 0.0, 1.0);
    float rayleighCoefficient = rayleigh - (1.0 * (1.0 - sunfade));
    vec3 betaR = totalRayleigh(primaries) * rayleighCoefficient;

    // Mie coefficient
    vec3 betaM = totalMie(primaries, mieKCoefficient, turbidity) * mieCoefficient;

    // Optical length, cutoff angle at 90 to avoid singularity
    float zenithAngle = acos(max(0.0, dot(UP, normalize(vWorldPosition - cameraPos))));
    float denom = cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0) / PI), -1.253);
    float sR = rayleighZenithLength / denom;
    float sM = mieZenithLength / denom;

    // Combined extinction factor
    vec3 Fex = exp(-(betaR * sR + betaM * sM));

    // In-scattering
    vec3 sunDirection = normalize(sunPosition);
    float cosTheta = dot(normalize(vWorldPosition - cameraPos), sunDirection);
    vec3 betaRTheta = betaR * rayleighPhase(cosTheta * 0.5 + 0.5);
    vec3 betaMTheta = betaM * henyeyGreensteinPhase(cosTheta, mieDirectionalG);
    float sunE = sunIntensity(dot(sunDirection, UP));
    vec3 Lin = pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0f - Fex), vec3(1.5));
    Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(0.5)), clamp(pow(1.0 - dot(UP, sunDirection), 5.0), 0.0, 1.0));

    // Composition + solar disc
    float sunAngularDiameterCos = cos(sunAngularDiameterDegrees);
    float sundisk = sunAngularDiameterCos; //  smoothstep(sunAngularDiameterCos, sunAngularDiameterCos + 0.00002, cosTheta);
    vec3 L0 = vec3(0.1) * Fex;
    L0 += sunE * 19000.0f * Fex * sundisk;
    vec3 texColor = Lin + L0;
    texColor *= 0.04;
    texColor += (vec3(0.0, 0.001, 0.0025) * 0.3f);

    // Tonemapping
    vec3 whiteScale = 1.0f / Uncharted2Tonemap(vec3(tonemapWeighting));
    vec3 curr = Uncharted2Tonemap(((float)log2(2.0 / pow(luminance, 4.0f))) * texColor);
    vec3 tone_color = curr * whiteScale;
    vec3 retColor = pow(tone_color, vec3(1.0 / (1.2 + (1.2 * sunfade))));
}