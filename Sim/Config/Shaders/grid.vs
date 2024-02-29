#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 perspective;

out vec3 fs_pos;

void main(void) {
    fs_pos = position;

    vec3 offset = vec3(gl_InstanceID, -gl_InstanceID, 0);

    gl_Position = perspective * view * vec4(position + offset, 1.0);
}
