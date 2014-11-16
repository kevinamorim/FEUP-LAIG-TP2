#ifndef _PATCH_H_
#define _PATCH_H_

#include <vector>
#include <string>

#include "Primitives.h"
#include "Point.h"
#include "MyMath.h"

using namespace std;

/*
	Patch
*/
class Patch : public Primitive {
public:
	Patch(int order, int partsU, int partsV, vector<Point3d *> controlPts, string drawingMode);
	Patch(int order, int partsU, int partsV, string drawingMode);

	void setControlPoints(float* ctrlPoints);
	void setNormals(float* n);
	void setColors(float* c);
	void setTexCoords(float* v);
	
	string Type();
	void draw();

protected:
	int order, partsU, partsV;

	string drawingMode;

	float* ctrlPoints;
	float* normals;
	float* colors;
	float* texCoords;
};

/*
	Plane
*/
class Plane : public Patch {
public:
	Plane(int parts);

	string Type();
	void draw();
};

#endif