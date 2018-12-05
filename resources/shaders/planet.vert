#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoords;

// UNIFORMS
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec3 PlanetColor; // = diffuse and ambient color

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;

uniform int ShaderMode;

// OUT
out vec3 pass_Normal;
out vec3 pass_VertexPosition; // is in camera-space?

out vec3 pass_Color;
out vec3 pass_LightPosition;
out vec3 pass_LightColor;
out float pass_LightIntensity;

out vec2 pass_TexCoords;

//TODO NormalMap

flat out int pass_ShaderMode;

void main(void)
{
	// Position of the Vertex that is rendered right now in clip space - is the output of the vertex shader
	//(transformed from object to world to view space to clip space by Transformation Matrices)
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	// Vertex Position without projection, as homogenous coordinate
	vec4 vertPos4 = (ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	// Vertex Position converted from homogenous view coordinate (in view space) to ndc (normalized device coordinates in normalized device space)
	pass_VertexPosition = vertPos4.xyz / vertPos4.w;
	
	// Normal
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	// TODO: calculate Normal with Normalmap
	
	// Colors and Lights
	pass_Color = PlanetColor;
	pass_LightPosition = (ViewMatrix*vec4(LightPosition,1.0f)).xyz;
	pass_LightColor = LightColor;
	pass_LightIntensity = LightIntensity;

	// Texture
	pass_TexCoords = in_TexCoords;

	// Shadermode
	pass_ShaderMode = ShaderMode;
}


