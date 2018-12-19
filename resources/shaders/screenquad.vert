#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes of VAO
layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_TexCoord;

// OUT
out vec2 pass_TexCoord;

void main(void)
{
	gl_Position = vec4(in_Position, 0.0, 1.0);
	pass_TexCoord = in_TexCoord;
}


