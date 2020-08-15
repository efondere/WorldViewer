#version 450 core

layout(location = 0) out vec4 color;

in vec3 v_barycentric;

//float edgeFactor()
//{
//    vec3 d = fwidth(v_barycentric);
//    vec3 a3 = smoothstep(vec3(0.0), d*1.5f, v_barycentric);
//    return min(min(a3.x, a3.y), a3.z);
//}

void main()
{
    //color = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);
    //color = vec4(1.f);
    //return;
//
    //if (any(lessThan(v_barycentric, vec3(0.02f))))
    //{
    //    color = vec4(0.f, 0.f, 0.f, 1.0f);
    //    return;
    //}
    //
    //color = vec4(0.6f, 0.6f, 0.6f, 1.f);

    color = vec4(v_barycentric, 1.0f);
}
