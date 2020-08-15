#version 450 core

layout(location = 0) in vec4 position;

out VertexInfo {
    vec4 modelPosition;
} vertexInfo;

uniform mat4 u_viewProjMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    vertexInfo.modelPosition = u_modelMatrix * position;
    gl_Position = u_viewProjMatrix * vertexInfo.modelPosition;
}
