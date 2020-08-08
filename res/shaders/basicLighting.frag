#version 330 core

layout(location = 0) out vec4 color;

in vec3  v_normal;
in vec3  v_fragPos;
in float v_height;
in vec4  v_eyeSpacePosition;

uniform vec3      u_lightPos;
uniform vec3      u_viewPos;
uniform vec3      u_lightColor;
uniform sampler2D u_texture;

uniform vec4  u_fogColor;
uniform float u_fogDensity;

float getFogFactor(float fogDensity, float fogCoord)
{
    const float end   = 70.f;
    const float start = 10.f;
    float result = 0.0;
    if (false)
        result = exp(-u_fogDensity * fogCoord);
    if (true)
        result = exp(-pow(u_fogDensity * fogCoord, 2.0));
    if (false)
        result = (end - fogCoord) / (end - start);

    return 1.0 - clamp(result, 0.0, 1.0);
}

void main()
{
    vec3 fragColor = vec3(texture(u_texture, vec2(0.5f, v_height / 255.f)));

    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_lightColor;

    // diffuse
    float diffuseStength = 1.0;
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStength * diff * u_lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * fragColor;
    //color = vec4(result, 0.2);
    color = vec4(result, 1.0);

    // calculate fog
    //if (v_height < 200.f)
    //{
        // we can also try linear fog
        float fogCoord = abs(v_eyeSpacePosition.z / v_eyeSpacePosition.w);
        color = mix(color, u_fogColor, getFogFactor(u_fogDensity /* * (1.0 - v_height / 200.f)*/, fogCoord) * 1.0); // (1.0 - v_height / 200.f) );
    //}

    //if (v_height < 25.f)
    //    color.a = 0.5f;
}
