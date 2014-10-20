#include "Point.h"

// =======================
//    POINT2D
// =======================
Point2d::Point2d() {
	this->x = 1.0;
	this->y = 1.0;
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

// =======================
//    POINT3D
// =======================
Point3d::Point3d() { 
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