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

float Point2d::distance(Point2d * a, Point2d * b)
{
	return sqrt(abs((b->x * b->x - a->x * a->x) + (b->y * b->y - a->y * a->y)));
}

float Point2d::dotProduct(Point2d * a, Point2d * b)
{
	return (a->x * b->x + a->y * b->y);
}

float Point2d::size()
{
	return sqrt(x * x + y * y);
}

// ostream overload
ostream& operator<<(ostream& os, const Point2d * point)
{
	os << "(" << point->x << ", " << point->y << ")";
	return os;
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

void Point3d::setPoint3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point3d::setPoint3d(Point3d * p)
{
	this->x = p->x;
	this->y = p->y;
	this->z = p->z;
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

float Point3d::distance(Point3d * a, Point3d * b)
{
	return sqrt(abs((b->x * b->x - a->x * a->x) + (b->y * b->y - a->y * a->y) + (b->z * b->z - a->z * a->z)));
}

float Point3d::size()
{
	return sqrt(x * x + y * y + z * z);
}

// ostream overload
ostream& operator<<(ostream& os, const Point3d * point)
{
	os << "(" << point->x << ", " << point->y << ", " << point->z << ")";
	return os;
}

// =======================
//    POINT4D
// =======================
Point4d::Point4d()
{ 
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Point4d::Point4d(float x, float y, float z, float w) : Point3d(x, y, z)
{
	this->w = w;
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

float Point4d::distance(Point4d * a, Point4d * b)
{
	return sqrt(abs((b->x * b->x - a->x * a->x) + (b->y * b->y - a->y * a->y) + (b->z * b->z - a->z * a->z) + (b->w * b->w - a->w * a->w)));
}

float Point4d::size()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

// ostream overload
ostream& operator<<(ostream& os, const Point4d * point)
{
	os << "(" << point->x << ", " << point->y << ", " << point->z << ", " << point->w << ")";
	return os;
}