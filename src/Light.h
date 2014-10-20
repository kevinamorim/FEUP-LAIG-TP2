#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "CGFlight.h"
#include "Point.h"

#include <string>

using namespace std;

/*
	Class that identifies a light source read from the anf file.
*/
class Light : public CGFlight {
public:
	Light(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point4d* ambient, Point4d* diffuse, Point4d* specular);
	~Light();

	bool Enabled();								// If true, a marker will be drawn in the scene at the position of the light source

	// Get Methods
	string getName();							// Returns the name of the light source (this is defined in the anf file has the 'id' attribute)

	// Set Methods
	void setID(unsigned int id);				// Sets the id the a light source. This is a GL_LIGHT_X (defined in openGl)
	void setEnabled(bool value);				// Sets the state of a light source : {enabled, disabled]

	// Virtual Methods
	virtual void updateLight() = 0;				// Updates the state of the light source : [position, direction(spot), angle(spot), exponent(spot)]
	virtual string Type() = 0;					// Returns the type of the light : [omni, spot]

	// Auxiliary Methods
	float* getFloat4v(float* v);				// Some openGl methods require a 3d vector to be a float[4] where result = (v[0], v[1], v[2], 1.0)
	float* subtractFloatv(float* v1, float* v2);// Returns a float[4] where result = (v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], 1.0)

protected:
	string name;	// name of the light source

	bool marker;	// indicator of wether this light is to have a representational sphere in the scene
};

class OmniLight : public Light {
public:
	OmniLight(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point4d* ambient, Point4d* diffuse, Point4d* specular);
	
	string Type();			// See the class Light

protected:

	void updateLight();		// See the class Light
};

class SpotLight : public Light {
public:
	SpotLight(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point3d* tar, float angle, float exp, Point4d* ambient, Point4d* diffuse, Point4d* specular);

	string Type();			// See the class Light

protected:
	void updateLight();		// See the class Light

	Point3d* target;		// point int 3d space that indicates the target point to where the spot light will be turned to

	float angle;			// angle of the spotlight (overture of the light's cone)
	float exponent;			// exponent of the spotlight (the bigger the exponent, the more the light will dim with the distance to the center of the light's cone)
};

#endif