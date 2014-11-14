#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include <GL\glut.h>

#include "Point.h"
#include "MyMath.h"

using namespace std;

class Animation {
protected:
	Animation(string id, float span);

	string id;
	float spanTime;
	
	unsigned long oldTime;

	bool restart;

public:
	virtual void init(unsigned long t) = 0;
	virtual void draw() = 0;
	virtual void update(unsigned long t) = 0;
	virtual void reset() = 0;

	string getID();
};

class LinearAnimation : public Animation {
public:
	LinearAnimation(string id, float span, vector<Point3d*> controlPts);

	void init(unsigned long t);
	void draw();
	void update(unsigned long t);
	void reset();

	//
	void move(float distance);
	Point3d * getAngle();

protected:
	int currentControl;
	vector<Point3d*> controlPoints;
	vector<Point3d*> direction;			// Point3d that indicates the direction of the current movement (Eg.: direction[1] = controlPoint[1] - controlPoint[0] <normalized>)
	vector<float> distance;
	Point3d * currentPos;
	Point3d * currentAngle;

	float speed;

	float moved;
};

class CircularAnimation : public Animation {
public:
	CircularAnimation(string id, float span, Point3d *center, float radius, float startAng, float rotAng);

	void init(unsigned long t);
	void draw();
	void update(unsigned long t);
	void reset();

protected:
	Point3d *center;
	float radius, startAngle, rotateAngle;

	float currentRotate, currentPos;
	float linearVelocity, angularVelocity;
};

#endif