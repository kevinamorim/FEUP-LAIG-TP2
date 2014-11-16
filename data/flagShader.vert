// Getting values from application

varying vec4 coords;

uniform float deltaTime;
uniform float wind;

const float PI = 3.14159265358979;

const float heightMultiplier = 0.04;
const float offsetMultiplier = 0.4;

void main() {
	
	float angle = gl_MultiTexCoord0.s * wind;
	float offset = offsetMultiplier * wind * deltaTime;

	vec4 newCoord = vec4(gl_Vertex.x, gl_Vertex.y + (sin((angle + offset) * PI) * heightMultiplier), gl_Vertex.z, 1.0);
	
	gl_Position = gl_ModelViewProjectionMatrix * newCoord;
	
	// set the RGB components of "gl_FrontColor" (built-in varying) to the XYZ components of the normal (and alpha=1)
	// these values will be received interpolated in the fragment shader as "gl_Color"
	gl_FrontColor = vec4(1.0, 1.0, 1.0 ,1.0);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	coords = newCoord * 4.0;
}