// Getting values from application

// Lighting
varying vec3 N;
varying vec3 v;

varying vec4 coords;

uniform float deltaTime;
uniform float wind;

const float PI = 3.14159265358979;

const float heightMultiplier = 0.6;
const float offsetMultiplier = 0.4;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

	float randomNr;

	if(wind == 0.0)
		randomNr = 0.0;
	else
		randomNr = rand(gl_MultiTexCoord0.st) * 0.2;
	
	float angleS = gl_MultiTexCoord0.s * wind;
	float offset = offsetMultiplier * (0.6 * wind) * deltaTime;

	vec4 newCoord = vec4(gl_Vertex.x, gl_Vertex.y + (sin((angleS + randomNr + offset) * PI) * heightMultiplier), gl_Vertex.z, 1.0);
	
	gl_Position = gl_ModelViewProjectionMatrix * newCoord;
	
	// set the RGB components of "gl_FrontColor" (built-in varying) to the XYZ components of the normal (and alpha=1)
	// these values will be received interpolated in the fragment shader as "gl_Color"
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	coords = newCoord * 4.0;

	
	// Lighting
	v = vec3(gl_ModelViewMatrix * newCoord);       
	N = normalize(gl_NormalMatrix * gl_Normal);  
}