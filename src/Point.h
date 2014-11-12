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

	static Point2d* subtract(Point2d *a, Point2d *b);

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

	static Point3d* subtract(Point3d *a, Point3d *b);

protected:
	float z;
};

class Point4d : public Point3d {
public:
	Point4d(float x, float y, float z, float w);

	float W();

	void setPoint4d(float x, float y, float z, float w);

	float* getFloatv();

	static Point4d* subtract(Point4d *a, Point4d *b);

protected:
	float w;
};

#endif