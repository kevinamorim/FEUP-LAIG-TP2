#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include "CGFobject.h"
#include "Appearance.h"
#include "Point.h"

#include <GL\glut.h>

const float PI = 3.14159265358979;
const float INV_PI = 0.3183098861838;
const int QUAD_RES = 10;

using namespace std;

/*
	Primitive

	Primitive parent class. Holds all the common properties and methods of each primitive. 
*/
class Primitive : public CGFobject {
public:

	virtual string Type() = 0;		/* Returns a string with the type of the primitive. */
	virtual void draw() = 0;		/* Draws the primitive to the screen. */
	void setTextureParams(float s, float t);	/* Sets the mapping coordinates for the texture. */

protected:

	Primitive();					/* Empty constructor */

	Point2d tex;					/* Texture mapping coordinates, where X = S and Y = T */
};

/*
	==========================================
					INHERITANCE
	==========================================

	All actually drawable primitives. All these primitives inherits properties and methods from 
	their parent class: Primitive.


	More primitives may be added.

*/

/*
	Triangle
*/
class Triangle : public Primitive {
public:

	Triangle(Point3d* point1, Point3d* point2, Point3d* point3);	/* Default constructor */

	// Inherited methods
	string Type();																								
	void draw();

private:

	// Members
	vector<Point3d *> verts;
	vector<Point2d *> texCoords;
	Point3d normal;

	float getSideLength(int side);		/* Returns length for a given side of the triangle. */
	void calculateNormal();				/* Calculates the normal for the triangle. */
	void calculateTextureCoords();		/* Calculates the texture coordinates for the triangle. */
};

/*
	Rectangle
*/
class Rectangle : public Primitive {
public:

	Rectangle(Point2d* p1, Point2d* p2);	/* Default constructor */

	// Inherited methods
	string Type();
	void draw();

private:

	vector<vector<Point2d*> > verts;		/* Vertex of the rectangle */
	float width, height;					/* Size of the rectangle. */

	void calculateVertex(Point2d* p1, Point2d* p2);	/* Calculates the vertex for the rectangle. */

};

/*
	Cylinder
*/
class Cylinder : public Primitive {
public:

	Cylinder(float base, float top, float height, int slices, int stacks); /* Default Constructor */

	// Inherited methods
	string Type();
	void draw();

private:

	float base;
	float top;
	float height;	
	int slices;		/* Number of slices, like a pizza, the cylinder has slices... */
	int stacks;		/* Number of vertical divisions of the cylinder */

	float alpha;	// angle for increment along the building of the cylinder vertex

	vector<vector<Point3d*> > verts;
	vector<vector<Point3d*> > norms;
	vector<vector<Point2d*> > texCoords;

	void calculateVertex();			/* Calculates the cylinder vertex */
	void calculateNormals();		/* Calculates the cylinder normals */
	void calculateTextureCoords();	/* Calculates the cylinder texture coords */
};

/*
	Sphere
*/
class Sphere : public Primitive {
public:
	Sphere(float radius, int slices, int stacks);

	string Type();
	void draw();

private:
	float radius;
	int slices;
	int stacks;

	GLUquadricObj* obj; 
};

/*
	Torus
*/
class Torus : public Primitive {
public:
	Torus(float inner, float outer, int slices, int loops);

	string Type();
	void draw();

private:
	float inner;
	float outer;
	float radiusTube;	// middle of the torus' tube.
	int slices;
	int loops;

	int numVerts;

	vector<vector<Point3d* >> verts;
	vector<vector<Point3d* >> norms;

	void calculate();		/* Calculates the verts and normals coordinates. */ 

};

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

private:
	CGFtexture *texture;

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
private:
};

#endif