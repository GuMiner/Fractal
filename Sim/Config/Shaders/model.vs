#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;

// Reproject the position and normals appropriately.
out vec4 fs_position;
out vec4 fs_normal;

void main() {
    vec4 fullPosition = view * model * vec4(position, 1.0);
    fs_position = vec3(fullPosition);
    fs_normal = vec3(normalize(normal * vec4(normal, 0.0)));

    gl_Position = projection * fullPosition;
}
