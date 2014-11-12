#include "Point.h"

// =======================
//    POINT2D
// =======================
Point2d::Point2d()
{
	this->x = 0;
	this->y = 0;
}

Point2d::Point2d(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Point2d::setPoint2d(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Point2d::X()
{
	return this->x;
}

float Point2d::Y()
{
	return this->y;
}

float* Point2d::getFloatv()
{
	float* floatv = new float[2];

	floatv[0] = x;
	floatv[1] = y;

	return floatv;
}

Point2d* Point2d::subtract(Point2d *a, Point2d *b)
{
	return new Point2d(a->x - b->x, a->y - b->y);
}

// =======================
//    POINT3D
// =======================
Point3d::Point3d()
{ 
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point3d::Point3d(float x, float y, float z) : Point2d(x, y)
{
	this->z = z;
}

float Point3d::Z()
{
	return this->z;
}

void Point3d::setPoint3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float* Point3d::getFloatv()
{
	float* floatv = new float[3];

	floatv[0] = x;
	floatv[1] = y;
	floatv[2] = z;

	return floatv;
}

Point3d* Point3d::subtract(Point3d *a, Point3d *b)
{
	return new Point3d(a->x - b->x, a->y - b->y, a->z - b->z);
}

// =======================
//    POINT4D
// =======================
Point4d::Point4d(float x, float y, float z, float w) : Point3d(x, y, z)
{
	this->w = w;
}

float Point4d::W()
{
	return this->w;
}

void Point4d::setPoint4d(float x, float y, float z, float w)
{
	this->setPoint3d(x, y, z);
	this->w = w;
}

float* Point4d::getFloatv()
{
	float* floatv = new float[4];

	floatv[0] = x;
	floatv[1] = y;
	floatv[2] = z;
	floatv[3] = w;

	return floatv;
}

Point4d* Point4d::subtract(Point4d *a, Point4d *b)
{
	return new Point4d(a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w);
}