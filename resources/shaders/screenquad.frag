#version 150

//IN
in vec2 pass_TexCoord;
uniform sampler2D FBTexture;


uniform bool Grayscale;
uniform bool HorizontalMirror;
uniform bool VerticalMirror;
uniform bool Blur;


// OUT
out vec4 out_color;

void main() {
	vec2 texCoords = pass_TexCoord;
	//horizontal mirroring
	if(HorizontalMirror) {
		texCoords.y = 1.0 - pass_TexCoord.y;	
	}


	//vertical mirroring
	if(VerticalMirror) {
		texCoords.x = 1.0 - pass_TexCoord.x;
	}

	out_color = texture(FBTexture, texCoords);

	//grayscale
	if(Grayscale) {
		float lum = 0.2126 * out_color.r + 0.7152 * out_color.g + 0.0722 * out_color.b;
    	out_color = vec4(lum, lum, lum, 0);
	}

	//blur
	if(Blur) {
		vec2 pixel_size = texCoords / gl_FragCoord.xy;
    	vec4 sum = vec4(0.0);

    	sum = texture(FBTexture, (vec2(texCoords.x - pixel_size.x, texCoords.y + pixel_size.y))) * 0.0625 
    	+ texture(FBTexture, (vec2(texCoords.x, texCoords.y + pixel_size.y))) * 0.125
    	+ texture(FBTexture, (vec2(texCoords.x + pixel_size.x, texCoords.y + pixel_size.y))) * 0.0625
		+ texture(FBTexture, (vec2(texCoords.x - pixel_size.x, texCoords.y))) * 0.125
		+ texture(FBTexture, (vec2(texCoords.x, texCoords.y))) * 0.25
		+ texture(FBTexture, (vec2(texCoords.x + pixel_size.x, texCoords.y))) * 0.125
 		+ texture(FBTexture, (vec2(texCoords.x - pixel_size.x, texCoords.y - pixel_size.y))) * 0.00625
 		+ texture(FBTexture, (vec2(texCoords.x , texCoords.y - pixel_size.y))) * 0.125
 		+ texture(FBTexture, (vec2(texCoords.x + pixel_size.x, texCoords.y - pixel_size.y))) * 0.0625;

    	out_color = sum;
	}
	/*
	vec2 texCoords = pass_TexCoord;
	out_color = texture(FBTexture, texCoords);
	*/
}

