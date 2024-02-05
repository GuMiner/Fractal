#version 330 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

uniform sampler2D normals;

// Adjust normals from the sampler
uniform mat4 normalMatrix;

in vec3 gs_position [];

// Reproject the position and normals appropriately.
out vec3 fs_normal;
out vec3 fs_position;

void main() {
    ivec2 normalsSize = textureSize(normals, 0);

    int y = gl_PrimitiveIDIn / normalsSize.x;
    int x = gl_PrimitiveIDIn % normalsSize.x;
    vec3 normal = texelFetch(normals, ivec2(x, y), 0).xyz;

    vec3 fs_normal_cache = vec3(normalize(normalMatrix * vec4(normal, 0.0)));

    // Don't modify the positions of the vertices
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        fs_position = gs_position[i];
        fs_normal = fs_normal_cache; // Must be written after every EmitVertex call
        EmitVertex();
    }

    EndPrimitive();
}
