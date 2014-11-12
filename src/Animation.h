#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include "Point.h"

using namespace std;

class Animation {
protected:
	Animation(string id, float span);

	string id;
	float spanTime;

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void update(unsigned long t) = 0;
	virtual void reset() = 0;

	bool restart;

public:
	string getID();
};

class LinearAnimation : public Animation {
public:
	LinearAnimation(string id, float span, vector<Point3d*> controlPts);

protected:
	void init();
	void draw();
	void update(unsigned long t);
	void reset();

	vector<Point3d*> controlPoints;
	int currentControl;

	Point3d * currentPos;
	Point3d * currentDir;
	float velocity;
};

class CircularAnimation : public Animation {
public:
	CircularAnimation(string id, float span, Point3d *center, float radius, float startAng, float rotAng);

protected:
	void init();
	void draw();
	void update(unsigned long t);
	void reset();

	Point3d *center;
	float radius, startAngle, rotateAngle;

	float currentRotate, currentPos;
	float linearVelocity, angularVelocity;
};

#endif