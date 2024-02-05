#version 330 core

layout (location = 0) in vec3 position;

// Projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 gs_position;

void main() {
    vec4 fullPosition = view * model * vec4(position, 1.0);

    gs_position = vec3(fullPosition);
    gl_Position = perspective * fullPosition;
}
