#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 perspective;

uniform int width;
uniform float tileSize;
uniform float time;

out float edgeFactor;
out vec3 fs_pos;

void main(void) {
    fs_pos = position;

    int x = gl_InstanceID % width - width / 2;
    int y = gl_InstanceID / width - width / 2;
    vec3 offset = vec3(x * tileSize, -y * tileSize, sin(x - time) * cos(y + time) * 10.0);

    edgeFactor = float(max(abs(x), abs(y))) / float(width);
    gl_Position = perspective * view * vec4(position + offset, 1.0);
}
