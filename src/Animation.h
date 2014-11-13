#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include <GL\glut.h>

#include "Point.h"

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

protected:
	vector<Point3d*> controlPoints;
	int currentControl;

	Point3d * currentPos;
	Point3d * currentDir;
	float velocity;
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