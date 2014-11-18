// Getting values from application

varying vec4 coords;

const float PI = 3.14159265358979;

const float heightMultiplier = 0.1;
const float offsetMultiplier = 0.4;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float angleS = gl_MultiTexCoord0.s;
	float angleT = gl_MultiTexCoord0.t;
	
	float randomNr = rand(gl_MultiTexCoord0.st) * 0.2;

	vec4 newCoord = vec4(gl_Vertex.x, gl_Vertex.y + (sin(abs(angleS + randomNr) * PI) * heightMultiplier), gl_Vertex.z, 1.0);
	
	//newCoord.y += (cos((angleT + offset) * PI) * heightMultiplier) * randomNr;
	
	gl_Position = gl_ModelViewProjectionMatrix * newCoord;
	
	// set the RGB components of "gl_FrontColor" (built-in varying) to the XYZ components of the normal (and alpha=1)
	// these values will be received interpolated in the fragment shader as "gl_Color"
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	coords = newCoord * 4.0;
}