#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

// Projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

// Simple pass-through position+color rendering
out vec4 fs_color;

void main(void) {
    vec4 fullPosition = view * model * vec4(position, 1.0);
    fs_color = color;

    gl_Position = perspective * fullPosition;
}