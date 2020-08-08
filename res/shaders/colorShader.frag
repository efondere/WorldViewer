#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

in float v_height;

void main()
{
    color = texture(u_texture, vec2(0.5f, v_height / 255.f));
}
