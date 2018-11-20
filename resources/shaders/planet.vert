#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec3 PlanetColor; // = diffuse and ambient color

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity; //?

out vec3 pass_Normal;
out vec3 pass_Color;

void main(void)
{
	// const:
	// vec3 SpecularColor = vec3(1,1,1);
	// PI
	// flaot Reflectivity
	// float Alpha
	

	//TODO
	// ß Funktion für Lichtquelle
	// Halfway Vector
	// Cd
	// Cs

	//Wie simple.vert:
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	
	// Was ist das? :
	// vec4vertPos4 = (ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	// Wie simple.vert:
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Color = PlanetColor;
}

