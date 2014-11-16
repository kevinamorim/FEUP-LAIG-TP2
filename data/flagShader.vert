// Getting values from application

uniform float time;

void main() {

	vec4 newCoord = vec4(gl_Vertex.x, gl_Vertex.y + (sin(gl_MultiTexCoord0.s * 6.28) * 0.1), gl_Vertex.z, 1.0);
	
	gl_Position = gl_ModelViewProjectionMatrix * newCoord;

	/*
	// set the RGB components of "gl_FrontColor" (built-in varying) to the XYZ components of the normal (and alpha=1)
	// these values will be received interpolated in the fragment shader as "gl_Color"
	gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
	*/
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}