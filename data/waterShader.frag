uniform sampler2D baseTexture;

varying vec4 coords;

const float colorMixHigh = 0.4;
const float colorMixLow = 0.1;

void main()
{
	// "coords" here is interpolated from the values passed from the FS.
	// Those are based on the original vertices' coordinates, without considering transformations.
	// Use "coords.x" to color the fragment differently if the original X coordinate is positive or negative;
	
	if(coords.y >= 0.0)
	{
		gl_FragColor = vec4(texture2D(baseTexture, gl_TexCoord[0].st).rgb, 0.8) + (coords.y * vec4(colorMixHigh, colorMixHigh, colorMixHigh, 1.0));
	}
	else
	{
		gl_FragColor = vec4(texture2D(baseTexture, gl_TexCoord[0].st).rgb, 0.4) + (coords.y * vec4(colorMixLow, colorMixLow, colorMixLow, 0.8));
	}
	

	//gl_FragColor.a = 0.2;
}
