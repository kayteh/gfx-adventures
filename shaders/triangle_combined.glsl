#include "Common.glsl"
#pragma vertex vert
#pragma fragment frag

in vec2 position;

out vec4 outColor;

uniform float u_Depth;

void vert(inout vec4 pos) {
  pos = vec4(position, 0.0, 1.0);
}

void frag() {
    vec3 sinTime = vec3(
        sin(0.3 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
        cos(0.6 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
        1-(sin(1 + u_Depth + u_Time.y * 0.001) + 1 * 0.5)
    );

    outColor = vec4(sinTime, 1.0);
}