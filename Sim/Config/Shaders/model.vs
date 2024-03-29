#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat4 normalMatrix;

// Reproject the position and normals appropriately.
out vec3 fs_position;
out vec3 fs_normal;

void main() {
    vec4 fullPosition = view * model * vec4(position, 1.0);
    fs_position = vec3(fullPosition);
    fs_normal = vec3(normalize(normalMatrix * vec4(normal, 0.0)));

    gl_Position = perspective * fullPosition;
}
