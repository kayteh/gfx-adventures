#version 150 core

uniform vec4 u_Time;
uniform vec4 u_SinTime;
// uniform float u_Position;

out vec4 outColor;

void main()
{
    outColor = vec4(u_SinTime.x/10, 1-(u_SinTime.y), 1.0, 1.0);
}