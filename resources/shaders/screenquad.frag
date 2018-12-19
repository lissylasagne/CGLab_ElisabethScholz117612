#version 150

//IN
in vec2 pass_TexCoord;
uniform sampler2D FBTexture;

// OUT
out vec4 out_color;

void main() {
	out_color = texture(FBTexture, pass_TexCoord);
}

