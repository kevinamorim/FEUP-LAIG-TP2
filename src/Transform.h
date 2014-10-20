#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "CGFobject.h"
#include "Point.h"

#include <stdio.h>

// Transformation class: will hold the information about a transformation.
class Transform
{
public:
	Transform();
	~Transform();

	bool isGood() { return good; }

	// Applies the transformation. 
	virtual void apply() = 0;

	virtual void print() = 0;

protected:
	bool good; // Indicates wether a transformation has been successfully created, so then we can choose what to do.

};

class Translate : public Transform {
public:
	Translate(Point3d* to);
	void apply();

	void print();

private:
	Point3d* to; // Position xyz to where the translation should be made
};

class Rotate : public Transform {
public:
	Rotate(char axis, float angle);
	void apply();

	void print();

private:
	char axis; // 'x', 'y', 'z' -> rotation axis
	float angle; // Rotation angle
};

class Scale : public Transform {
public:
	Scale(Point3d* factor);
	void apply();

	void print();

private:
	Point3d* factor; // Scaling factor (x, y, z)
};

#endif