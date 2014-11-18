uniform sampler2D baseTexture;

varying vec4 coords;

const float colorMix = 1.2;

void main()
{
	// "coords" here is interpolated from the values passed from the FS.
	// Those are based on the original vertices' coordinates, without considering transformations.
	// Use "coords.x" to color the fragment differently if the original X coordinate is positive or negative
	gl_FragColor = texture2D(baseTexture, gl_TexCoord[0].st) + (coords.y * vec4(colorMix, colorMix, colorMix, 1.0));
}
