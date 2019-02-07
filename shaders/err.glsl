#pragma vertex vert
#pragma fragment frag

in vec3 in_Position;

void vert(inout vec4 pos) {
  pos = vec4(in_Position, 1.0);
}

vec4 frag() {
  return vec4(1.0, 0.0, 1.0, 1.0);
}