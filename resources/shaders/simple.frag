#version 150

in  vec3 pass_Normal;
in vec3 pass_Color;
out vec4 out_Color;

void main() {
  out_Color = vec4(abs(normalize(pass_Color+pass_Normal)), 1.0);
}

