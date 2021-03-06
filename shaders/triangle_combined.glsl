// example of a combined shader.
/**
 * My preprocessor has 3 directives it parses before
 *  GLSL ever sees it.
 *
 * - #include "filename"
 *    this replaces the line with the contents of another file
 *    ALWAYS PROCESSED LAST. #pragmas inside includes are not processed.
 * - #pragma (vertex|fragment) <function>
 *    and the magic happens here, see below.
 * - #pragma glsl <version string>
 *    suggests a version string to use, as it will be added 
 *     before some other magic lines.
 * - #pragma blend <blend mode>
 * - #pragma stencil <stencil mode>
 *    (not implemented) flags to the material renderer for blending
 *    and stenciling.
 * 
 * So, the magic. You might notice we never define or use 
 *  gl_Position or the out color directive, or even a main(). 
 *  One is only defined for vertex shaders, and the other 
 *  is useless but confusing from a black box perspective.
 * To combat this, I inject code that generates a main()
 *  that calls the pragma defined functions, with a special
 *  signature.
 *
 * Fragment function is: vec4 function(T[, bool]); which returns the out color
 *  Similar to HLSL's `: COLOR' return directive. This is passed into
 *  an `out' field. Optional bool is for facing/VFACE.
 * Vertex function is: T vert(inout vec4 pos); which pos is passed 
 *  directly to gl_Position. Other gl_* can be used with a guard.
 * T in both of these must be a struct. void may be possible in the future.
 *
 * I find that having an `in' global directive in the fragment doesn't
 *  actually affect anything, and I assume it gets compiled out.
 *  but vert to frag ins/outs are VERY specific.
 *
 * Two guards are used for anything the compiler doesn't like,
 *  VERTEX and FRAGMENT which are defined directly after #version
 *  (so second line before the rest of the shader)
 */
#include "Common.glsl"
#pragma vertex vert
#pragma fragment frag


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