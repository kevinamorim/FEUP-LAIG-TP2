#ifndef _POINT_H_
#define _POINT_H_

#include <cmath>
#include <iostream>

using namespace std;

class Point2d {
public:
	Point2d();
	Point2d(float x, float y);

	void setPoint2d(float x, float y);

	float* getFloatv();

	static Point2d* subtract(Point2d *a, Point2d *b);
	static float distance(Point2d *a, Point2d *b);
	

	float size();

	//
	float x, y;

	// ostream overload
	friend ostream& operator<<(ostream& os, const Point2d * point);
	
	Point2d* operator=(const Point2d * point)
	{
		this->x = point->x;
		this->y = point->y;
		return this;
	}
};

class Point3d : public Point2d {
public:
	Point3d();
	Point3d(float x, float y, float z);

	void setPoint3d(float x, float y, float z);
	void setPoint3d(Point3d * p);

	float* getFloatv();

	static Point3d* subtract(Point3d *a, Point3d *b);
	static float distance(Point3d * a, Point3d * b);

	float size();

	//
	float z;

	// ostream overload
	friend ostream& operator<<(ostream& os, const Point3d * point);

	Point3d* operator=(const Point3d * point)
	{
		float x = point->x;
		float y = point->y;
		float z = point->z;

		return new Point3d(x, y, z);
	}
};

class Point4d : public Point3d {
public:
	Point4d();
	Point4d(float x, float y, float z, float w);

	void setPoint4d(float x, float y, float z, float w);

	float* getFloatv();

	static Point4d* subtract(Point4d *a, Point4d *b);
	static float distance(Point4d * a, Point4d * b);

	float size();

	//
	float w;

	// ostream overload
	friend ostream& operator<<(ostream& os, const Point4d * point);
};

#endif