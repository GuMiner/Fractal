#version 330 core

layout (location = 0) in vec2 position;

out vec3 fs_pos;

void main(void)
{
    // TODO Doesn't account for camera rotation
    fs_pos = normalize(vec3(position, 1)); // FOV == 30
    
    // 1.0f for Z corresponds to the far plane
    gl_Position = vec4(position, 1.0f, 1.0f);
}
