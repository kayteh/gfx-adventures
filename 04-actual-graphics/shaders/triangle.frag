#version 150 core

#include "Common.glsl"

out vec4 outColor;

void main()
{
    // vec3 sinTime = vec3(
    //     sin(0.3 + u_Time.y * 0.001) + 1 * 0.5,
    //     cos(0.6 + u_Time.y * 0.001) + 1 * 0.5,
    //     1-(sin(1 + u_Time.y * 0.001) + 1 * 0.5)
    // );

    // outColor = vec4(sinTime, 1.0);
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}