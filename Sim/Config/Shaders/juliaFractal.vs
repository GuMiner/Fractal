#version 330 core

layout (location = 0) in vec2 position;

out vec2 fs_pos;

// Pass-Thru to do all rendering in the fragment shader
void main(void)
{
    fs_pos = position;
    
    // 1.0f for Z corresponds to the far plane
    gl_Position = vec4(position, 1.0f, 1.0f);
}
