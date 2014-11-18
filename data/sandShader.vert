// Getting values from application
uniform sampler2D baseTexture;
uniform sampler2D heightMap;

// Lighting
varying vec3 N;
varying vec3 v;

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

	vec4 newCoord = vec4(gl_Vertex.xyz, 1.0);

	vec4 color = texture2D(baseTexture, gl_MultiTexCoord0.st);
	vec4 height = texture2D(heightMap, gl_MultiTexCoord0.st);
	
	newCoord.y += (color.r + color.g + color.b - 2.0) * 2.0;
	newCoord.y += (height.r + height.g + height.b) * 3.0;
	
	gl_Position = gl_ModelViewProjectionMatrix * newCoord;
	
	// set the RGB components of "gl_FrontColor" (built-in varying) to the XYZ components of the normal (and alpha=1)
	// these values will be received interpolated in the fragment shader as "gl_Color"
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	coords = newCoord * 4.0;
	
	// Lighting
	v = vec3(gl_ModelViewMatrix * newCoord);       
	N = normalize(gl_NormalMatrix * gl_Normal);  
}