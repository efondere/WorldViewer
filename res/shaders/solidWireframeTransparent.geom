#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexInfo {
    vec4 modelPosition;
} shapeInfo[];

out vec3 v_barycentric;

void main()
{
    //vec2 e0 = shapeInfo[0].modelPosition.xz - shapeInfo[1].modelPosition.xz;
    //vec2 e1 = shapeInfo[1].modelPosition.xz - shapeInfo[2].modelPosition.xz;
    //vec2 e2 = shapeInfo[2].modelPosition.xz - shapeInfo[0].modelPosition.xz;
//
    //// h0 is the projection of e1 over a unit vector that is perpendicular to e0
    //float h0 = abs(dot(normalize(vec2(e0.y, -e0.x)), e1));
    //float h1 = abs(dot(normalize(vec2(e1.y, -e1.x)), e2));
    //float h2 = abs(dot(normalize(vec2(e2.y, -e2.x)), e0));

    float h0 = 1.f;
    float h1 = 1.f;
    float h2 = 1.f;

    v_barycentric = vec3(0, h1, 0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    v_barycentric = vec3(0, 0, h2);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    v_barycentric = vec3(h0, 0, 0);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();    
}
