#version 330 core

in float edgeFactor;
out vec4 color;

void main(void)
{
    
    color = mix(vec4(0.5, 0.8, 0.5, 1.0), vec4(1.0), edgeFactor * 2);
}