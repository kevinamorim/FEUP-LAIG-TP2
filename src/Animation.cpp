#include "Animation.h"

float distance(Point3d * a, Point3d * b);
float distance(vector<Point3d *> pts);


// =======================
//	Animation
// =======================
Animation::Animation(string id, float span)
{
	this->id = id;
	this->spanTime = span;
}

// =======================
//	Linear Animation
// =======================
LinearAnimation::LinearAnimation(string id, float span, vector<Point3d *> controlPts) : Animation(id, span)
{
	this->controlPoints = controlPts;
	this->velocity = distance(controlPoints) / span;

	this->reset();
}

void LinearAnimation::draw()
{
	// TODO
}

void LinearAnimation::reset()
{
	this->currentPos = this->controlPoints[0];
	this->currentControl = 0;
}

void LinearAnimation::update(unsigned long t)
{
	// TODO
}

// =======================
//	Circular Animation
// =======================
CircularAnimation::CircularAnimation(string id, float span, Point3d * center, float radius, float startAng, float rotAng) : Animation(id, span)
{
	this->center = center;
	this->radius = radius;
	this->startAngle = startAng;
	this->rotateAngle = rotAng;

	// TODO : velocities

	this->reset();
}

void CircularAnimation::draw()
{

}

void CircularAnimation::reset()
{
	this->currentRotate = 0 ;// TODO
	this->currentPos = 0; // TODO
}

void CircularAnimation::update(unsigned long t)
{
	// TODO
}

// =======================
// =======================
float distance(Point3d * a, Point3d * b)
{
	return sqrt((b->X() * b->X() - a->X() * a->X()) + (b->Y() * b->Y() - a->Y() * a->Y()) + (b->Z() * b->Z() - a->Z() * a->Z()));
}

float distance(vector<Point3d *> pts)
{
	float dist = 0;

	unsigned int size = pts.size();

	for(unsigned int i = 1; i < size; i++)
	{
		dist += distance(pts[i-1], pts[i]); 
	}

	return dist;
}