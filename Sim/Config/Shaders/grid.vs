#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 view;
uniform mat4 perspective;

uniform int width;
uniform float tileSize;
uniform float time;

out float edgeFactor;

out vec3 fs_position;
out vec3 fs_normal;

void main(void) {
    int x = gl_InstanceID % width - width / 2;
    int y = gl_InstanceID / width - width / 2;
    vec3 offset = vec3(x * tileSize, -y * tileSize, sin(x - time) * cos(y + time) * 10.0);

    vec4 offsetPosition = vec4(position + offset, 1.0f);
    fs_position = (view * offsetPosition).xyz;

    // Must be computed here because each instance has a different offset.
    mat4 positionAsMatrix = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        offsetPosition);
    mat4 normalMatrix = transpose(inverse(view * positionAsMatrix));

    fs_normal = vec3(normalize(normalMatrix * vec4(normal, 0.0)));

    edgeFactor = float(max(abs(x), abs(y))) / float(width);
    gl_Position = perspective * view * offsetPosition;
}
