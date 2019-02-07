#include "Common.glsl"
#pragma vertex vert
#pragma fragment frag

in vec3 position;

uniform float u_Depth;

void vert(inout vec4 pos) {
  pos = vec4(position, 1.0);
}

vec4 frag() {
  vec3 sinTime = vec3(
    sin(0.3 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
    cos(0.6 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
    1-(sin(1 + u_Depth + u_Time.y * 0.001) + 1 * 0.5)
  );

  return vec4(sinTime, 1.0);
}