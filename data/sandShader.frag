uniform sampler2D baseTexture;

// Lighting
varying vec3 N;
varying vec3 v;

varying vec4 coords;

void main()
{
	// "coords" here is interpolated from the values passed from the FS.
	// Those are based on the original vertices' coordinates, without considering transformations.
	// Use "coords.x" to color the fragment differently if the original X coordinate is positive or negative;
	
	gl_FragColor = texture2D(baseTexture, gl_TexCoord[0].st);
	
	// Lighting
	// vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	// vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);  
	// Idiff = clamp(Idiff, 0.0, 1.0); 

	// gl_FragColor *= 1.5 * Idiff.a;
}
