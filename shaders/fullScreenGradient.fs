#version 400 core

out vec4 color;

in vec2 fs_pos;

// Render a Julia fractal on the background
void main(void)
{
	color = vec4((fs_pos.xy + vec2(1.0, 1.0)) / 2, 0.0f, 1.0f);
}