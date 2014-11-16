#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "CGFobject.h"
#include "Texture.h"

#include "Point.h"
#include "MyMath.h"

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

	void setTexture(Texture * tex);	/* Sets the mapping coordinates for the texture. */

protected:

	Primitive();					/* Empty constructor */

	bool hasTexture;
	Texture *texture;
	Point2d texParams;				/* Texture mapping coordinates, where X = S and Y = T */
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

protected:

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

protected:

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

protected:
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

protected:
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

protected:
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

#endif