#version 330 core

// convert a triangle mesh to a wireframe
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

// There should be a better logic for this

void main() {
    //if (gl_in[0].gl_Position.x == gl_in[1].gl_Position.x) // top triangle
    //{
    //    //gl_Position = gl_in[2].gl_Position;
    //    //EmitVertex();
//
    //    gl_Position = gl_in[0].gl_Position;
    //    EmitVertex();
//
    //    gl_Position = gl_in[1].gl_Position;
    //    EmitVertex();
    //}
    //else // bottom triangle
    //{
    //    gl_Position = gl_in[0].gl_Position;
    //    EmitVertex();
//
    //    gl_Position = gl_in[2].gl_Position;
    //    EmitVertex();
    //}

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
