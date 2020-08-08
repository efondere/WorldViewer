#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3  v_fragPos;
out vec3  v_normal;
out vec4  v_eyeSpacePosition;
out float v_height;

uniform mat4 u_MVP;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;

void main()
{

    v_fragPos = vec3(u_modelMatrix * position);
    v_normal = mat3(transpose(inverse(u_modelMatrix))) * normal;

    gl_Position = u_MVP * position;
    v_height = position.y;

    v_eyeSpacePosition = u_viewMatrix * u_modelMatrix * vec4(position.xyz, 1.0);
}
