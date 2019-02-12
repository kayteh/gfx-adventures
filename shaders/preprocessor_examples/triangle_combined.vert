#version 410 core
#define VERTEX
// INCLUDED FROM: Common.glsl

uniform vec4 u_Time;
uniform vec4 u_SinTime;
uniform vec4 u_CosTime;
uniform vec4 u_DeltaTime;

uniform vec3 u_LightPos;
// -- END Common.glsl



struct v2f {
  vec2 uv0;
};

in vec3 in_Position;
in vec2 in_Texcoord0;
uniform float u_Depth;

v2f vert(inout vec4 pos) {
  v2f i;
  pos = vec4(in_Position, 1.0);
  i.uv0 = in_Texcoord0;
  return i;
}

vec4 frag(v2f i, bool vface) {
  vec3 sinTime = vec3(
    sin(0.3 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
    cos(0.6 + u_Depth + u_Time.y * 0.001) + 1 * 0.5,
    1-(sin(1+u_Depth + u_Time.y * 0.001) + 1 * 0.5)
  );
  // return vec4(1-i.uv0, 0.0, 1.0);
  return vec4(sinTime, 1.0);
}
// -- Generated Vertex
out v2f _VERT2FRAG;
void main() {
  _VERT2FRAG = vert(gl_Position);
}
