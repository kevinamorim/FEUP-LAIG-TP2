#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "CGFobject.h"
#include "Point.h"

#include <stdio.h>

// Transformation class: will hold the information about a transformation.
class Transform
{
public:
	// Applies the transformation. 
	virtual void apply() = 0;

protected:
	bool good; // Indicates wether a transformation has been successfully created, so then we can choose what to do.

};

class Translate : public Transform {
public:
	Translate(Point3d* to);

	void apply();

private:
	Point3d* to; // Position xyz to where the translation should be made
};

class Rotate : public Transform {
public:
	Rotate(char axis, float angle);

	void apply();

private:
	char axis; // 'x', 'y', 'z' -> rotation axis
	float angle; // Rotation angle
};

class Scale : public Transform {
public:
	Scale(Point3d* factor);

	void apply();

private:
	Point3d* factor; // Scaling factor (x, y, z)
};

#endif