#version 150

//IN
in vec3 pass_Normal;
in vec3 pass_VertexPosition;
in vec3 pass_Color;

in vec3 pass_LightPosition;
in vec3 pass_LightColor;
in float pass_LightIntensity;

// OUT
out vec4 out_Color;

void main() {
	// Const
	float pi = 3.1415926;
	
	// Parameters
	vec3 ambient_color = pass_Color.xyz;
	vec3 diffuse_color = pass_Color;
	vec3 specular_color = vec3(1.0f,1.0f,1.0f);

	//rho
	float reflectivity = 0.9;
	//4*alpha
	float glossiness = 32;

	// Vectors
	// Normal on Surface Vector
	vec3 N = normalize(pass_Normal);

	// Light-Vertex-Vector
	vec3 L = normalize(pass_LightPosition - pass_VertexPosition);

	// Vertex-to-Camera-Vector
	vec3 V = normalize(-pass_VertexPosition);

 	//Halfway-Vector
 	vec3 H = normalize(L+V);

	//Diffuse Effect (Lambertian BRDF)
	//if Angle < 0, set to 0
	float diffuseAngle = max(dot(L, N), 0.0f);
	vec3 diffuse_effect = diffuse_color * diffuseAngle * (reflectivity / pi);

	//Specular Effect (Blinn-Phong BRDF)
	//if Angle < 0, set to 0 
	float specularAngle = max(dot(H,V), 0.0f);
	vec3 specular_effect = specular_color * pow(specularAngle, glossiness);

	// All together
	vec3 total_color = ambient_color + diffuse_effect + specular_effect;

	out_Color = vec4(total_color.xyz, 1.0f);

}
