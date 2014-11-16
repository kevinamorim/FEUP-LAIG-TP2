#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "CGFapplication.h"

#include "Point.h"
#include "MyMath.h"

using namespace std;

class Animation {
public:
	
	virtual void draw() = 0;
	virtual void update(unsigned long t) = 0;
	virtual void reset() = 0;
	virtual void init(unsigned long t) = 0;

	bool Done();
	bool inLoop;

	string getID();

	Point3d * getCurrentPos();

protected:
	Animation(string id, float span);

	string id;
	float spanTime;
	
	unsigned long oldTime;

	bool restart;
	bool done;

	Point3d * currentPos;
};

class LinearAnimation : public Animation {
public:
	LinearAnimation(string id, float span, vector<Point3d*> controlPts);

	void draw();
	void update(unsigned long t);
	void reset();
	void init(unsigned long t);

	//
	void move(float distance);

protected:
	int currentControl;
	int numControlPoints;
	vector<Point3d*> controlPoints;
	vector<Point3d*> direction;			// Point3d that indicates the direction of the current movement (Eg.: direction[1] = controlPoint[1] - controlPoint[0] <normalized>)
	
	vector<float> distance;

	Point3d * currentRotation;
	Point3d * getRotation();

	float speed;
	float moved;
};

class CircularAnimation : public Animation {
public:
	CircularAnimation(string id, float span, Point3d *center, float radius, float startAng, float rotAng);

	void draw();
	void update(unsigned long t);
	void reset();
	void init(unsigned long t);

	Point3d * getCurrentPos();

protected:
	Point3d *center;
	float radius, startAngle, rotateAngle;

	float currentRotate;
	float angularSpeed;
};

// Plays a series of animations in sequence
class ComposedAnimation : public Animation {
public:
	ComposedAnimation(string id);

	void draw();
	void update(unsigned long t);
	void reset();

	void addAnimation(Animation *anim);

protected:
	void init(unsigned long t);

	int currentAnimation;
	int numAnimations;
	vector<Animation *> animations;

	//Point3d* offsetPos;	// Relative position to the end of the last animation played
};

#endif