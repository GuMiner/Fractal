#version 400 core
#include "fullScreenQuad/gammaCorrection.fs_frag"

in vec2 fs_pos;

uniform vec2 c;
uniform float time;
uniform sampler1D fractalGradient;
uniform int maxIterations;

out vec4 color;

// Render a Julia fractal on the background
void main(void)
{
    // Rendering colors just based on positions.
    //vec2 fraction = (fs_pos.xy + vec2(1.0, 1.0)) / 2;
    //vec4 colorLookup = mix(texture(fractalGradient, fraction.x), texture(fractalGradient, fraction.y), 0.5f);
	//color = vec4(colorLookup.xyz, 1.0f);
    
	float dist = 0.50f;
	float speed = 1.0f;
	vec2 point = vec2(cos(time*speed) * dist, sin(time * speed) * dist);
	
    vec2 z = vec2(fs_pos.x * 2, fs_pos.y * 2);
    int iterations = 0;
    float thresholdSqd = 4.0f;
    while (iterations < maxIterations && dot(z, z) < thresholdSqd)
    {
        vec2 zSqd = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + point;
        z = zSqd + c;
        ++iterations;
    }
    
    if (iterations == maxIterations || iterations == 0)
    {
        color = vec4(0, 0, 0, 1);
    }
    else
    {
        color = vec4(texture(fractalGradient, float(iterations) / float(maxIterations)).xyz, 1.0f);
    }
}