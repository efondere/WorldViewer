#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_projection * u_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
} 

#shader fragment

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_texture;

void main()
{    
    FragColor = texture(u_texture, TexCoords);
}
