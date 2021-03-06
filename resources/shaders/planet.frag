#version 150

//IN
in vec3 pass_Normal;
in vec3 pass_VertexPosition;
in vec3 pass_Color;

in vec3 pass_LightPosition;
in vec3 pass_LightColor;
in float pass_LightIntensity;

//Texture Coordinates from vertex shader and PlanetTexture Uniform
in vec2 pass_TexCoords;
uniform sampler2D pass_PlanetTexture;

flat in int pass_ShaderMode;

// OUT
out vec4 out_Color;

void main() {

	// Const
	float pi = 3.1415926;
	
	// Parameters
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;

	if(pass_ShaderMode == 2){
		// using PlanetColor
		ambient_color = pass_Color.xyz * 0.01;
		diffuse_color = pass_Color;
		specular_color = pass_Color * 1.2;//pass_LightColor;	
	}

	// Shadermode 3 is Texture-Shadermode
	else if (pass_ShaderMode == 3){
		//using Texture Color, calculated from Texture Coordinate and Texture
		vec4 textureColor = texture(pass_PlanetTexture, pass_TexCoords);
		ambient_color = textureColor.xyz * 0.01;
		diffuse_color = textureColor.xyz * 0.1;
		specular_color = textureColor.xyz * 0.5;
	}
	else{
		// using PlanetColor
		ambient_color = pass_Color.xyz * 0.01;
		diffuse_color = pass_Color;
		specular_color = pass_Color * 1.2;//pass_LightColor;	
	}

	//rho
	float reflectivity = 1.0;
	//4*alpha
	float glossiness = 16;

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

	vec3 beta = pass_LightIntensity * pass_LightColor / 4*pi * pow(length(pass_VertexPosition - pass_LightPosition), 2);

	// All together
	vec3 total_color = ambient_color + (diffuse_effect + specular_effect) * beta;

	vec3 homogColor = vec3(0.0f, 0.0f, 0.0f);

	// CELL SHADING
	if(pass_ShaderMode == 2){
		// calculate the angle between the vector from the vertex position to the
		// light source and the normal on the surface (aka the lambertian)
	    float lambertian = max(dot(L, normalize(pass_Normal)), 0.0);

	    // depending on the the angle between light source and normal the lambertian value is discretized, we use the function that maps values to <steps> levels of brightness
	    float steps = 4.0f;
	    lambertian = floor(lambertian*steps)/(steps-1);

	    // the angle between normal and view
		float angleNV = dot(-normalize(pass_VertexPosition), normalize(pass_Normal));

		// if normal and view are orthogonal (edge of planet) use outline color
		if(abs(angleNV) < 0.3f){
			homogColor = vec3(1.0f, 1.0f, 1.0f);
		}
		else{
			homogColor = ambient_color + lambertian * diffuse_color + specular_effect;
		}
	}

	// NORMALES SHADING
	else{ homogColor = total_color;}

	// Gamma-Correction (with Screen-Gamma 2.2)
	out_Color = vec4(homogColor, 1.0);
	out_Color = normalize(out_Color); //normalizing for better visibility
	//out_Color = texture(pass_PlanetTexture, pass_TexCoords);
	//vec4(pow(homogColor, vec3(1.0/2.2)), 1.0);
}

