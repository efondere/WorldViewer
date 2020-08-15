#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VertexInfo {
    vec3 normal;
} ShapeInfo[];

const float MAGNITUDE = 2.0;

uniform mat4 u_projMatrix;

void generate_normal_line(int index)
{
    gl_Position = u_projMatrix * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = u_projMatrix * (gl_in[index].gl_Position + vec4(ShapeInfo[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    for (int i = 0; i < 3; i++)
    {
        generate_normal_line(i);
    }
}
