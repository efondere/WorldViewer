#version 330 core

layout (location = 0) in vec3 position;
layout (location = 0) in vec3 normal;

out VertexInfo {
    vec3 normal;
} vertexInfo;

uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_viewMatrix * u_modelMatrix * vec4(position, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));
    vertexInfo.normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));
}
