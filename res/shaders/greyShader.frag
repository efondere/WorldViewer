#version 330 core

// https://stackoverflow.com/questions/25274783/opengl-3-3-glsl-fragment-shader-fog-effect-not-working
// http://www.mbsoftworks.sk/tutorials/opengl3/12-fog-outside/

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
}
