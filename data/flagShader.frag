uniform sampler2D baseTexture;

void main()
{
	// "coords" here is interpolated from the values passed from the FS.
	// Those are based on the original vertices' coordinates, without considering transformations.
	// Use "coords.x" to color the fragment differently if the original X coordinate is positive or negative

	gl_FragColor = texture2D(baseTexture, gl_TexCoord[0].st);
	
	/*
	if (coords.x > 0.0)
	{
		// The built-in "gl_Color" is interpolated from "gl_FrontColor"'s of the vertices 
		// originating this fragment.
		gl_FragColor = gl_Color;
	}
	else
	{
		// use the absolute value of the xyz coordinates as color values 
		// (here divided by three as that is the dimension of the teapot being used in this example)
		gl_FragColor.rgb = abs(coords.xyz) / 3.0;
		gl_FragColor.a = 1.0;
	}
	*/
}
