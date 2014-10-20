#ifndef _POINT_H_
#define _POINT_H_

class Point2d {
public:
	Point2d();
	Point2d(float x, float y);

	float X();
	float Y();

	void setPoint2d(float x, float y);

	float* getFloatv();

protected:
	float x, y;
};

class Point3d : public Point2d {
public:
	Point3d();
	Point3d(float x, float y, float z);

	float Z();

	void setPoint3d(float x, float y, float z);

	float* getFloatv();

protected:
	float z;
};

class Point4d : public Point3d {
public:
	Point4d(float x, float y, float z, float w);

	float W();

	void setPoint4d(float x, float y, float z, float w);

	float* getFloatv();

protected:
	float w;
};

#endif