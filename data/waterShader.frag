uniform sampler2D baseTexture;

// Lighting
varying vec3 N;
varying vec3 v;

varying vec4 coords;

const float colorMix = 0.1;

void main()
{
	gl_FragColor = vec4(texture2D(baseTexture, gl_TexCoord[0].st).rgb, 1.0);
	gl_FragColor += (0.8 * coords.y * vec4(colorMix, colorMix, colorMix, 1.0));
	//gl_FragColor = vec4(gl_LightSource[0].diffuse);
	
	
	// Lighting
	// vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	// vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);  
	// Idiff = clamp(Idiff, 0.0, 1.0); 

	// gl_FragColor *= 1.5 * Idiff.a;
   
}
